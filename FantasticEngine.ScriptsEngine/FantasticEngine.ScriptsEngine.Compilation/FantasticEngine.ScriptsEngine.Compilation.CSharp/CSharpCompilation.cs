using FantasticEngine.ScrptsEngine;
using System;
using System.IO;
using System.Collections.Generic;
using System.Text;
using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.CSharp;
using Microsoft.CodeAnalysis.Emit;
using FantasticEngine.ScriptsEngine.Compilation.CSharp;
using System.Reflection;
using System.Xml;

namespace FantasticEngine.ScriptsEngine.Compilation
{
    public class CSharpCompilation : ICompilation
    {
        #region mono
        /// <summary>
        /// 使用Mono动态编译
        /// <para>https://www.cnblogs.com/zhongzf/archive/2011/11/27/2264955.html</para>
        /// <para>https://www.mono-project.com/docs/about-mono/languages/csharp/</para>
        /// </summary>
        [Obsolete("不兼容.NetStandard")]
        public static Assembly CompileByMono(string scriptsPath, bool isDebug = false, string outputFile = null, string librariesPath = null)
        {
            throw new PlatformNotSupportedException("Only on .Net Framework.");
        }
        #endregion

        #region roslyn
        /// <summary>
        /// 使用Roslyn动态编译
        /// <para>https://blog.csdn.net/Crazy2910/article/details/106918516</para>
        /// </summary>
        public virtual ScriptsAnalysis Build(string projectPath, string outputPath, bool isDebug = false, string librariesPath = null)
        {
            //路径检查
            outputPath = CheckOutputFilePath(outputPath);
            string assemblyName = Path.GetFileNameWithoutExtension(outputPath);
            librariesPath = FEPath.GetFullPath(librariesPath);

            //获取文件
            string[] scripts = Directory.GetFiles(FEPath.GetFullPath(projectPath), "*.cs", SearchOption.AllDirectories);
            string[] dlls = Directory.GetFiles(FEPath.GetFullPath(librariesPath), "*.dll", SearchOption.AllDirectories);

            //读取需要忽略的dll
            List<string> ignoreDlls = GetIgnoreReferences();
            ignoreDlls.Add(Path.GetFileName(outputPath));

            //源码读取
            LinkedList<SyntaxTree> synctaxTrees = new LinkedList<SyntaxTree>();
            foreach (string file in scripts)
            {
                string script = File.ReadAllText(file, Encoding.UTF8);
                SyntaxTree synctaxTree = CSharpSyntaxTree.ParseText(script, CSharpParseOptions.Default, file, Encoding.UTF8);
                synctaxTrees.AddLast(synctaxTree);
            }
            //synctaxTrees.AddFirst(CSharpSyntaxTree.ParseText(GenerateAssemblyInfo(assemblyName)));

            //添加用于编译的引用
            List<MetadataReference> references = new List<MetadataReference>(dlls.Length);
            foreach (string file in dlls)
            {
                if (ignoreDlls.Contains(Path.GetFileName(file)))
                {
                    continue;
                }
                //FConsole.Write(Path.GetFileName(file));
                MetadataReference reference = MetadataReference.CreateFromFile(file);
                references.Add(reference);
            }

            //编译设置
            CSharpCompilationOptions options = new CSharpCompilationOptions(
                outputKind: OutputKind.DynamicallyLinkedLibrary,
                optimizationLevel: (isDebug ? OptimizationLevel.Debug : OptimizationLevel.Release),
                platform: Platform.AnyCpu,
                moduleName: Path.GetFileNameWithoutExtension(outputPath)
            //publicSign: true
            );

            //创建编译器
            Microsoft.CodeAnalysis.CSharp.CSharpCompilation compilation = Microsoft.CodeAnalysis.CSharp.CSharpCompilation.Create(assemblyName, synctaxTrees, references, options);

            string filePath = Compile(compilation, outputPath, isDebug);
            if (!string.IsNullOrEmpty(filePath))
            {
                return new ScriptsAnalysis(new List<string> { filePath });
            }
            return null;
        }

        //编译
        protected string Compile(Microsoft.CodeAnalysis.Compilation compilation, string outputFilePath, bool isDebug)
        {
            //编译
            EmitResult result = compilation.Emit(outputFilePath, isDebug ? GetPdbFilePath(outputFilePath) : null);

            //判断
            if (!result.Success)
            {
                FEConsole.WriteErrorFormatWithCategory(Categories.COMPILATION, "Failed to compile C# scripts.");
                foreach (Diagnostic diagnostic in result.Diagnostics)
                {
                    if (diagnostic.WarningLevel == 0)
                    {
                        FEConsole.WriteErrorFormatWithCategory(Categories.COMPILATION, diagnostic.GetMessage());
                    }
                }
                return null;
            }
            FEConsole.WriteInfoFormatWithCategory(Categories.COMPILATION, "C# compiled successfully.");
            return Path.Combine(outputFilePath, compilation.AssemblyName);
        }
        #endregion

        /// <summary>
        /// 获取符号表的路径
        /// </summary>
        protected string GetPdbFilePath(string assemblyPath)
        {
            return assemblyPath.Substring(0, assemblyPath.Length - 4) + ".pdb";
        }

        #region .net framework
        /// <summary>
        /// 编译cs文件
        /// <para>https://learn.microsoft.com/zh-cn/dotnet/api/microsoft.csharp.csharpcodeprovider</para>
        /// </summary>
        [Obsolete("不兼容NetStandard")]
        public static Assembly CompileByCodeDom(string scriptsPath, bool isDebug = false, string outputFile = null, string librariesPath = null)
        {
            throw new PlatformNotSupportedException("Only on .Net Framework.");
            //CodeDomProvider provider = new CSharpCodeProvider();
            //if (provider == null)
            //{
            //    FConsole.WriteErrorWithCategory(Categories.COMPILER, "Failed to generate CSharpCodeProvider.");
            //    return null;
            //}

            //if (string.IsNullOrEmpty(outputFile))
            //{
            //    outputFile = "FantasticEngine.Runtime.dll";
            //}
            //if (!outputFile.EndsWith(".dll"))
            //{
            //    outputFile += ".dll";
            //}
            //CompilerParameters options = new CompilerParameters();
            //options.GenerateExecutable = false;
            //options.GenerateInMemory = false;
            //options.TreatWarningsAsErrors = false;
            //options.OutputAssembly = outputFile;
            //if (string.IsNullOrEmpty(librariesPath))
            //{
            //    librariesPath = Environment.CurrentDirectory;
            //}
            //else
            //{
            //    librariesPath = Path.Combine(Environment.CurrentDirectory, librariesPath);
            //}

            //string[] dlls = Directory.GetFiles(librariesPath, "*.dll");
            //foreach (string dll in dlls)
            //{
            //    options.ReferencedAssemblies.Add(Path.Combine(librariesPath, Path.GetFileName(dll)));
            //}

            //CompilerResults result = provider.CompileAssemblyFromSource(options, scriptsPath);
            //if (result.Errors.Count > 0)
            //{
            //    FConsole.WriteErrorWithCategory(Categories.COMPILER, "Failed to compile C# scripts.");
            //    foreach (CompilerError error in result.Errors)
            //    {
            //        FConsole.WriteErrorWithCategory(Categories.COMPILER, "  {0}", error.ToString());
            //    }
            //    return null;
            //}
            //FConsole.WriteInfoWithCategory(Categories.COMPILER, "C# compiled successfully.");
            //return result.CompiledAssembly;
        }
        #endregion

        /// <summary>
        /// 输出路径检查
        /// </summary>
        private static string CheckOutputFilePath(string outputFile)
        {
            if (string.IsNullOrEmpty(outputFile))
            {
                outputFile = "FantasticEngine.Server.Runtime.dll";
            }
            if (!outputFile.EndsWith(".dll"))
            {
                outputFile += ".dll";
            }
            return FEPath.GetFullPath(outputFile);
        }

        /// <summary>
        /// 生成版本字符串
        /// </summary>
        /// <param name="assemblyName"></param>
        /// <returns></returns>
        private static string GenerateAssemblyInfo(string assemblyName)
        {
            StringBuilder builder = new StringBuilder();
            builder.AppendLine("using System.Reflection;");
            builder.AppendLine("using System.Runtime.CompilerServices;");
            builder.AppendLine("using System.Runtime.InteropServices;");

            builder.AppendLine($"[assembly: AssemblyTitle(\"{assemblyName}\")]");
            builder.AppendLine($"[assembly: AssemblyProduct(\"{assemblyName}\")]");
            builder.AppendLine($"[assembly: AssemblyCopyright(\"Copyright © FantasticEngine.\")]");
            builder.AppendLine($"[assembly: ComVisible(false)]");
            builder.AppendLine($"[assembly: Guid(\"{Guid.NewGuid().ToString()}\")]");
            builder.AppendLine($"[assembly: AssemblyVersion(\"1.0.0.1\")]");
            return builder.ToString();
        }

        private static List<string> GetIgnoreReferences()
        {
            string xmlFileName = "FantasticEngine.ScriptsEngine.CSharp.Ignored.References.config";
            if (!File.Exists(xmlFileName))
            {
                return new List<string>();
            }
            XmlReaderSettings settings = new XmlReaderSettings();
            settings.IgnoreComments = true;
            XmlDocument xml = new XmlDocument();
            using (XmlReader reader = XmlReader.Create(xmlFileName, settings))
            {
                xml.Load(reader);
            }
            XmlNode node = xml.SelectSingleNode("references");
            List<string> dlls = new List<string>(node.ChildNodes.Count + 1);
            foreach (XmlNode child in node)
            {
                dlls.Add(child.InnerText);
            }
            return dlls;
        }

    }
}
