using System;
using System.Collections.Generic;
using System.Text;

namespace FantasticEngine.ScriptsEngine.Common
{
    public interface IGenerator
    {
        void Generate(ScriptsAnalysis analysis);
    }
}
