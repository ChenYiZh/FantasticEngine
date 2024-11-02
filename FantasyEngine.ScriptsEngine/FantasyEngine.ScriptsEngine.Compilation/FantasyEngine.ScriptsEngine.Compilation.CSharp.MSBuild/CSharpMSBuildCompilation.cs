using FantasyEngine.ScriptsEngine.Compilation.CSharpMSBuild;
using Microsoft.Build.Evaluation;
using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.CSharp;
using Microsoft.CodeAnalysis.Diagnostics;
using Microsoft.CodeAnalysis.MSBuild;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Reflection;
using System.Security.Cryptography.Xml;
using System.Text;
using System.Threading.Tasks;

namespace FantasyEngine.ScriptsEngine.Compilation
{
    public class CSharpMSBuildCompilation : CSharpCompilation
    {
        public override ScriptsAnalysis Build(string projectPath, string outputPath, bool isDebug = false, string librariesPath = null)
        {
            string[] solutionFiles = Directory.GetFiles(projectPath, "*.sln", SearchOption.TopDirectoryOnly);
            if (solutionFiles != null && solutionFiles.Length > 0)
            {
                using (MSBuildWorkspace workspace = MSBuildWorkspace.Create())
                {
                    List<string> filePaths = new List<string>();
                    Task<Solution> taskLoading = workspace.OpenSolutionAsync(solutionFiles[0]);
                    taskLoading.Wait();
                    Solution solution = taskLoading.Result;
                    foreach (Microsoft.CodeAnalysis.Project project in solution.Projects)
                    {
                        string dllPath = CreateAssembly(project, outputPath, isDebug);
                        if (!string.IsNullOrEmpty(dllPath))
                        {
                            filePaths.Add(dllPath);
                        }
                    }
                    if (filePaths.Count > 0)
                    {
                        return new WorkspaceAnalysis(workspace, filePaths);
                    }
                    return null;
                }
            }
            else
            {
                string[] projFiles = Directory.GetFiles(projectPath, "*.csproj", SearchOption.TopDirectoryOnly);
                if (projFiles != null && projFiles.Length > 0)
                {
                    List<string> filePaths = new List<string>();
                    using (MSBuildWorkspace workspace = MSBuildWorkspace.Create())
                    {
                        foreach (string projFile in projFiles)
                        {
                            Task<Microsoft.CodeAnalysis.Project> taskLoading = workspace.OpenProjectAsync(projFile);
                            taskLoading.Wait();
                            Microsoft.CodeAnalysis.Project project = taskLoading.Result;
                            string dllPath = CreateAssembly(project, outputPath, isDebug);
                            if (!string.IsNullOrEmpty(dllPath))
                            {
                                filePaths.Add(dllPath);
                            }
                        }
                        if (filePaths.Count > 0)
                        {
                            return new WorkspaceAnalysis(workspace, filePaths);
                        }
                        return null;
                    }
                }
            }
            return base.Build(projectPath, outputPath, isDebug, librariesPath);
        }

        private string CreateAssembly(Microsoft.CodeAnalysis.Project project, string outputPath, bool isDebug)
        {
            CSharpCompilationOptions options = project.CompilationOptions as CSharpCompilationOptions;
            if (options == null)
            {
                options = new CSharpCompilationOptions(OutputKind.DynamicallyLinkedLibrary);
            }
            options = options.WithOptimizationLevel(isDebug ? OptimizationLevel.Debug : OptimizationLevel.Release);
            Task<Microsoft.CodeAnalysis.Compilation> compilingTask = project.WithCompilationOptions(options).GetCompilationAsync();
            compilingTask.Wait();
            Microsoft.CodeAnalysis.Compilation compilation = compilingTask.Result;

            return Compile(compilation, FPath.GetFullPath(outputPath), isDebug);
        }

        private ProjectInfo CreateProjectInfo(Microsoft.CodeAnalysis.Project project, string projectPath, string outputPath, bool debug)
        {
            if (project == null) { return null; }
            CSharpCompilationOptions options = project.CompilationOptions as CSharpCompilationOptions;
            if (options == null)
            {
                options = new CSharpCompilationOptions(OutputKind.DynamicallyLinkedLibrary);
            }
            options.WithOptimizationLevel(debug ? OptimizationLevel.Debug : OptimizationLevel.Release);
            return ProjectInfo.Create(project.Id,
                project.Version,
                project.Name,
                project.AssemblyName,
                LanguageNames.CSharp,
                project.FilePath,
                Path.Combine(outputPath, project.AssemblyName),
                options,
                project.ParseOptions,
                project.Documents.Select(d => CreateDocumentInfo(d)),
                project.ProjectReferences,
                project.MetadataReferences,
                project.AnalyzerReferences,
                project.AdditionalDocuments.
                Select(d => CreateDocumentInfo(d)),
                project.IsSubmission);
        }

        private DocumentInfo CreateDocumentInfo(TextDocument document)
        {
            return DocumentInfo.Create(document.Id,
                document.Name,
                document.Folders,
                SourceCodeKind.Regular,
                filePath: document.FilePath);
        }

        private DocumentInfo CreateDocumentInfo(Document document)
        {
            return DocumentInfo.Create(document.Id,
                document.Name,
                document.Folders,
                document.SourceCodeKind,
                filePath: document.FilePath);
        }
    }
}
