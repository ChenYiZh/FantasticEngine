using FantasyEngine.ScriptsEngine;

namespace FantasyEngine.ScrptsEngine
{
    public interface ICompilation
    {
        /// <summary>
        /// 编译
        /// </summary>
        /// <param name="projectPath">项目路径</param>
        /// <param name="outputFile">输出文件路径</param>
        /// <param name="isDebug">是否以Debug输出</param>
        /// <param name="librariesPath">引用库的路径</param>
        /// <returns>是否编译成功</returns>
        ScriptsAnalysis Build(string projectPath, string outputPath, bool isDebug = false, string librariesPath = null);
    }
}