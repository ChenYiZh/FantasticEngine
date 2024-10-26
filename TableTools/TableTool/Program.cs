/****************************************************************************
THIS FILE IS PART OF Fantasy Engine PROJECT
THIS PROGRAM IS FREE SOFTWARE, IS LICENSED UNDER MIT

Copyright (c) 2024 ChenYiZh
https://space.bilibili.com/9308172

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
****************************************************************************/

using System;
using System.IO;
using System.Linq;
using System.Text;
using FantasyEngine.Security;
using FantasyEngine.TableTool.Exporter;
using FantasyEngine.TableTool.Framework;
using FantasyEngine.TableTool.Generator;
using FantasyEngine.TableTool.Reader.Excel;

namespace FantasyEngine.TableTool
{
    enum EPlatform
    {
        None,
        Unity,
        Unreal
    }

    class Program
    {
        static void Main(string[] args)
        {
            Encoding.RegisterProvider(CodePagesEncodingProvider.Instance);
            FConsole.RegistLogger(new Logger());

            try
            {
                FConsole.Write("正在读取表数据...");
                IDatabase database = new ExcelDatabaseBuilder().Build("../");

                FConsole.Write("正在生成服务器表...");
                DatabaseExporter exporter = new DatabaseExporter("../Server", ".txt", true);
                exporter.Target = ETarget.SERVER;
                exporter.Export(database, new TextTableHeaderBuilder(), new TextTableBodyBuilder());

                FConsole.Write("正在生成客户端表...");
                exporter = new DatabaseExporter("../Client/Tables", ".txt", true);
                exporter.Target = ETarget.CLIENT;
                exporter.Export(database, new TextTableBodyBuilder()); //未混淆数据用来给策划做比对
                exporter = new DatabaseExporter("../Client/Tables", ".zip", true);
                exporter.Target = ETarget.CLIENT;
                exporter.Export(database, new TextTableBodyBuilder(), new TextContextFastObfuscatedBuilder()); //混淆后的数据

                FConsole.Write("正在生成MD5校验文件...");
                string[] files = Directory.GetFiles("../Client/Tables", "*.zip", SearchOption.TopDirectoryOnly);
                StringBuilder builder = new StringBuilder();
                foreach (string file in files)
                {
                    string line = string.Format("{0}|{1}", Path.GetFileNameWithoutExtension(file),
                        Md5Utility.GetMd5(File.ReadAllBytes(file)));
                    builder.AppendLine(line);
                }

                File.WriteAllText("../Client/assets.idx", builder.ToString(), new UTF8Encoding(false));

                FConsole.Write("正在生成客户端表结构...");
                JsonTableExporter.Export(database, "../Client/tables.json");

                if (args == null)
                {
                    goto end;
                }

                EPlatform platform = EPlatform.None;
                if (args.Contains("unity"))
                {
                    platform = EPlatform.Unity;
                }
                else if (args.Contains("ue"))
                {
                    platform = EPlatform.Unreal;
                }

                foreach (string arg in args)
                {
                    if (arg.StartsWith("server="))
                    {
                        string serverPath = arg.Substring("server=".Length);

                        FConsole.Write("正在生成服务器读表代码...");
                        IScriptsExporter scriptsExporter = new UnityScriptsExporter(ETarget.SERVER,
                            serverPath + "/CsScript/Data/GameTables.a.cs");
                        scriptsExporter.Generate(database);

                        FConsole.Write("正在导出服务器表数据...");
                        string serverTablePath = serverPath + "/Config/Tables";
                        if (!Directory.Exists(serverTablePath))
                        {
                            Directory.CreateDirectory(serverTablePath);
                        }

                        string[] serverTableFiles = Directory.GetFiles(serverTablePath);
                        foreach (string file in serverTableFiles)
                        {
                            File.Delete(file);
                        }

                        string[] serverTables = Directory.GetFiles("../Server");
                        foreach (string file in serverTables)
                        {
                            File.Copy(file, serverTablePath + "/" + Path.GetFileName(file));
                        }
                    }
                    else if (arg.StartsWith("client="))
                    {
                        string clientPath = arg.Substring("client=".Length);
                        IScriptsExporter scriptsExporter = null;
                        switch (platform)
                        {
                            case EPlatform.Unreal:
                                {
                                    string[] folders = Directory.GetDirectories(Path.Combine(clientPath, "Source"));
                                    if (folders == null || folders.Length == 0)
                                    {
                                        FConsole.WriteError("无法生成UE客户端代码，因为该项目没有包含C++工程！！");
                                        FConsole.WriteError("判断为 “项目路径/Source/工程目录”");
                                        goto end;
                                    }

                                    string protectName = Path.GetFileName(folders[0].Replace('/', '\\'));
                                    UEScriptsExporter ueExporter = new UEScriptsExporter(clientPath);
                                    scriptsExporter = ueExporter;
                                    ueExporter.ProjectName = protectName;
                                }
                                break;
                            case EPlatform.Unity:
                                scriptsExporter = new UnityScriptsExporter(ETarget.CLIENT,
                                    clientPath + "/Assets/Scripts/Data/GameTables.a.cs");
                                break;
                        }

                        if (scriptsExporter != null)
                        {
                            FConsole.Write("正在生成客户端读表代码...");
                            scriptsExporter.Generate(database);
                        }

                        FConsole.Write("正在导出客户端表数据...");
                        switch (platform)
                        {
                            case EPlatform.Unreal:
                                {
                                    string outFolder = clientPath + "/Content/Resources";
                                    if (Directory.Exists(outFolder + "/Tables"))
                                    {
                                        Directory.Delete(outFolder + "/Tables", true);
                                    }

                                    Directory.CreateDirectory(outFolder + "/Tables");
                                    File.Copy("../Client/assets.idx", outFolder + "/assets.idx", true);
                                    string[] clientOutTables = Directory.GetFiles("../Client/Tables");
                                    foreach (string outTable in clientOutTables)
                                    {
                                        File.Copy(outTable, outFolder + "/Tables/" + Path.GetFileName(outTable));
                                    }
                                }
                                break;
                            case EPlatform.Unity:
                                {
                                    string outFolder = clientPath + "/_Tables";
                                    if (Directory.Exists(outFolder))
                                    {
                                        Directory.Delete(outFolder, true);
                                    }

                                    Directory.CreateDirectory(outFolder);
                                    Directory.CreateDirectory(outFolder + "/Tables");

                                    File.Copy("../Client/assets.idx", outFolder + "/assets.idx", true);

                                    string[] clientOutTables = Directory.GetFiles("../Client/Tables");
                                    foreach (string outTable in clientOutTables)
                                    {
                                        File.Copy(outTable, outFolder + "/Tables/" + Path.GetFileName(outTable));
                                    }

                                    if (Directory.Exists(clientPath + "/Assets/Resources/Tables/"))
                                    {
                                        Directory.Delete(clientPath + "/Assets/Resources/Tables/", true);
                                    }
                                    Directory.CreateDirectory(clientPath + "/Assets/Resources/Tables/");

                                    File.Copy("../Client/assets.idx", clientPath + "/Assets/Resources/assets.txt", true);

                                    string[] clientResTables = Directory.GetFiles("../Client/Tables", "*.zip");
                                    foreach (string resTable in clientResTables)
                                    {
                                        File.Copy(resTable,
                                            clientPath + "/Assets/Resources/Tables/" + Path.GetFileNameWithoutExtension(resTable) + ".txt");
                                    }
                                }
                                break;
                        }
                    }
                }

            end:
                FConsole.Write("导表完成");
                FConsole.Write("任意键退出");
            }
            catch (Exception e)
            {
                FConsole.WriteException(e);
            }

            Console.ReadKey();
        }
    }
}