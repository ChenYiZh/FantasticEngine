using System;
using System.Collections.Generic;
using System.Text;

namespace FantasyEngine.ScriptsEngine
{
    /// <summary>
    /// 代码分析类，暂时支持持C#
    /// </summary>
    public class ScriptsAnalysis
    {
        /// <summary>
        /// Dll路径
        /// </summary>
        public List<string> FilePaths { get; protected set; }
        public ScriptsAnalysis(List<string> filePaths)
        {
            FilePaths = filePaths;
        }
    }
}
