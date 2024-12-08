/****************************************************************************
THIS FILE IS PART OF Fantasy Engine PROJECT
THIS PROGRAM IS FREE SOFTWARE, IS LICENSED UNDER MIT

Copyright (c) 2022-2030 ChenYiZh
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
using FantasyEngine.IO;
using FantasyEngine.Log;
using FantasyEngine.ScriptsEngine;
using FantasyEngine.ScriptsEngine.Compilation;
using System;
using System.Collections.Generic;
using System.IO;
using System.Reflection;
using System.Text;

namespace FantasyEngine.Network.Server
{
    internal class Configeration
    {
        private static bool _initialized = false;
        /// <summary>
        /// 初始化
        /// </summary>
        public static void Initialize()
        {
            if (_initialized)
            {
                return;
            }
            _initialized = true;

            //加载配置
            Settings.LoadFromFile();

            //打印启动信息
            RuntimeHost.PrintStartInfo();
            //return;
            //编译脚本
            if (!string.IsNullOrWhiteSpace(Settings.CSScriptsPath) && Directory.Exists(FEPath.GetFullPath(Settings.CSScriptsPath)))
            {
                FEConsole.WriteInfoFormatWithCategory(Categories.FOOLISH_SERVER, "Compiling scripts...");
                ScriptsAnalysis analysis = new CSharpCompilation().Build(Settings.CSScriptsPath, Settings.AssemblyName, Settings.IsDebug);
                if (analysis == null)
                {
                    throw new Exception("There is some errors on compiling the scripts.");
                }
            }

            //IL代码注入
            FEConsole.WriteInfoFormatWithCategory(Categories.FOOLISH_SERVER, "Checking models...");
            if (ILInjection.InjectEntityChangeEvent())
            {
                FEConsole.WriteInfoFormatWithCategory(Categories.FOOLISH_SERVER, "Some models have been modified automatically.");
                //if (string.Equals(Path.GetFileName(Assembly.GetEntryAssembly().GetName().Name + ".dll"), Settings.AssemblyName, StringComparison.OrdinalIgnoreCase))
                //{
                //    RuntimeHost.Reboot();
                //}
            }

            //加载程序集
            Assembly assembly;
            if ((assembly = AssemblyService.Load(Settings.AssemblyName)) == null)
            {
                FEConsole.WriteWarnFormatWithCategory(Categories.FOOLISH_SERVER, "Failed to load the assembly: " + Settings.AssemblyName);
            }

            FEConsole.WriteInfoFormatWithCategory(Categories.FOOLISH_SERVER, "Ready to start servers...");
        }
    }
}
