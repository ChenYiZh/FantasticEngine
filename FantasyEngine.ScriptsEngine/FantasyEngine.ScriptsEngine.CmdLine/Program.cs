using FantasyEngine;
using FantasyEngine.Log;
using FantasyEngine.ScriptsEngine;
using FantasyEngine.ScriptsEngine.Compilation;

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
            ScriptsAnalysis analysis = new CSharpMSBuildCompilation().Build("D:\\UEProjects\\FantasyEngine\\Experiments\\FantasyEngine.ScriptsEngine\\Test", "D:\\CSharpTest", true);
            foreach (string path in analysis.FilePaths)
            {
                FEConsole.Write(path);
            }
            Console.Read();
        }
    }
}
