using System;
using FantasyEngine.Log;

namespace FantasyEngine.TableTool
{
    public class Logger : ILogger
    {
        public void SaveLog(string level, string message)
        {
            switch (level)
            {
                case LogLevel.DEBUG:
                    Console.ForegroundColor = ConsoleColor.White;
                    break;
                case LogLevel.INFO:
                    Console.ForegroundColor = ConsoleColor.DarkCyan;
                    break;
                case LogLevel.WARN:
                    Console.ForegroundColor = ConsoleColor.DarkYellow;
                    break;
                case LogLevel.ERROR:
                    Console.ForegroundColor = ConsoleColor.DarkRed;
                    break;
                default:
                    Console.ForegroundColor = ConsoleColor.DarkGray;
                    break;
            }

            Console.WriteLine(message);
        }
    }
}