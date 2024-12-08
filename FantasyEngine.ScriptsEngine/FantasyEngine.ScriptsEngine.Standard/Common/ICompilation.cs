using FantasyEngine.ScriptsEngine;

namespace FantasyEngine.ScrptsEngine
{
    public interface ICompilation
    {
        /// <summary>
        /// ����
        /// </summary>
        /// <param name="projectPath">��Ŀ·��</param>
        /// <param name="outputFile">����ļ�·��</param>
        /// <param name="isDebug">�Ƿ���Debug���</param>
        /// <param name="librariesPath">���ÿ��·��</param>
        /// <returns>�Ƿ����ɹ�</returns>
        ScriptsAnalysis Build(string projectPath, string outputPath, bool isDebug = false, string librariesPath = null);
    }
}