using FantasticEngine;
using FantasticEngine.Log;
using FantasticEngine.ScriptsEngine;
using FantasticEngine.ScriptsEngine.Compilation;

namespace CmdLine
{
    internal class Program
    {
        class Logger : ILogger
        {
            public void SaveLog(string level, string message)
            {
                if (level == LogLevel.ERROR)
                {

                    Console.ForegroundColor = ConsoleColor.Red;
                }
                Console.WriteLine(message);
                Console.ForegroundColor = ConsoleColor.White;
            }
        }
        static void Main(string[] args)
        {
            FEConsole.RegistLogger(new Logger());
            FEConsole.Write("Hello, World!");
            ScriptsAnalysis analysis = new CSharpMSBuildCompilation().Build("D:\\UEProjects\\FantasticEngine\\Experiments\\FantasticEngine.ScriptsEngine\\Test", "D:\\CSharpTest", true);
            foreach (string path in analysis.FilePaths)
            {
                FEConsole.Write(path);
            }
            Console.Read();
        }
    }
}
