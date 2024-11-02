using Microsoft.CodeAnalysis;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FantasyEngine.ScriptsEngine.Compilation
{
    internal class WorkspaceAnalysis : ScriptsAnalysis
    {
        public Workspace Workspace { get; private set; }

        public WorkspaceAnalysis(Workspace workspace, List<string> filePaths) : base(filePaths)
        {
            Workspace = workspace;
        }
    }
}
