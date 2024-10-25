/****************************************************************************
THIS FILE IS PART OF Fantasy Engine PROJECT
THIS PROGRAM IS FREE SOFTWARE, IS LICENSED UNDER MIT

Copyright (c) 2024 ChenYiZh
https://space.bilibili.com/9308172

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
****************************************************************************/
using FantasyEngine.Collections;
using FantasyEngine.Common;
using FantasyEngine.Timer;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Reflection;
using System.Text;

namespace FantasyEngine.Log
{
    /// <summary>
    /// 输出类
    /// </summary>
    public class FConsole
    {
        /// <summary>
        /// 是否输出堆栈
        /// </summary>
        public static bool LogStackTracker { get; set; } = false;

        /// <summary>
        /// 默认类别名称
        /// </summary>
        public static string CATEGORY { get; set; } = "Log";

        /// <summary>
        /// 需要输出堆栈的Level
        /// </summary>
        public static IList<string> LogStackLevels { get; private set; } = new ThreadSafeList<string>() { LogLevel.ERROR };

        private static HashSet<ILogger> loggers = new HashSet<ILogger>();

        /// <summary>
        /// 已注册的Logger
        /// </summary>
        public static IReadOnlyCollection<ILogger> Loggers { get { return loggers; } }

        private static readonly object _syncRoot = new Object();

        //static FConsole()
        //{
        //    SyncRoot = new Object();
        //    loggers = new HashSet<ILogger>();
        //}

        /// <summary>
        /// 注册Logger
        /// </summary>
        /// <param name="logger"></param>
        /// <returns></returns>
        public static bool RegistLogger(ILogger logger)
        {
            bool result = false;
            lock (_syncRoot)
            {
                result = loggers.Add(logger);
            }
            return result;
        }

        /// <summary>
        /// 移除Logger
        /// </summary>
        /// <param name="logger"></param>
        /// <returns></returns>
        public static bool RemoveLogger(ILogger logger)
        {
            bool result = false;
            lock (_syncRoot)
            {
                result = loggers.Remove(logger);
            }
            return result;
        }

        /// <summary>
        /// 输出到Debug目录
        /// </summary>
        public static void Write(object message)
        {
            SendMessage(LogLevel.DEBUG, CATEGORY, message?.ToString(), true);
        }
        /// <summary>
        /// 输出到Debug目录
        /// </summary>
        public static void WriteWithCategory(string category, object message)
        {
            SendMessage(LogLevel.DEBUG, category, message?.ToString(), true);
        }

        /// <summary>
        /// 输出到Debug目录
        /// </summary>
        public static void WriteFormat(string message, params object[] args)
        {
            SendMessage(LogLevel.DEBUG, CATEGORY, Format(message, args), true);
        }
        /// <summary>
        /// 输出到Debug目录
        /// </summary>
        public static void WriteFormatWithCategory(string category, string message, params object[] args)
        {
            SendMessage(LogLevel.DEBUG, category, Format(message, args), true);
        }
        /// <summary>
        /// 输出到Info目录
        /// </summary>
        public static void WriteInfo(object message)
        {
            SendMessage(LogLevel.INFO, CATEGORY, message?.ToString(), true);
        }
        /// <summary>
        /// 输出到Info目录
        /// </summary>
        public static void WriteInfoWithCategory(string category, object message)
        {
            SendMessage(LogLevel.INFO, category, message?.ToString(), true);
        }
        /// <summary>
        /// 输出到Info目录
        /// </summary>
        public static void WriteInfoFormat(string message, params object[] args)
        {
            SendMessage(LogLevel.INFO, CATEGORY, Format(message, args), true);
        }
        /// <summary>
        /// 输出到Info目录
        /// </summary>
        public static void WriteInfoFormatWithCategory(string category, string message, params object[] args)
        {
            SendMessage(LogLevel.INFO, category, Format(message, args), true);
        }
        /// <summary>
        /// 输出到Warn目录
        /// </summary>
        public static void WriteWarn(object message)
        {
            SendMessage(LogLevel.WARN, CATEGORY, message?.ToString(), true);
        }
        /// <summary>
        /// 指定类别输出到Warn目录
        /// </summary>
        public static void WriteWarnWithCategory(string category, object message)
        {
            SendMessage(LogLevel.WARN, category, message?.ToString(), true);
        }
        /// <summary>
        /// 输出到Warn目录
        /// </summary>
        public static void WriteWarnFormat(string message, params object[] args)
        {
            SendMessage(LogLevel.WARN, CATEGORY, Format(message, args), true);
        }
        /// <summary>
        /// 指定类别输出到Warn目录
        /// </summary>
        public static void WriteWarnFormatWithCategory(string category, string message, params object[] args)
        {
            SendMessage(LogLevel.WARN, category, Format(message, args), true);
        }
        /// <summary>
        /// 输出到Exception目录
        /// </summary>
        public static void WriteError(object message)
        {
            SendMessage(LogLevel.ERROR, CATEGORY, message?.ToString(), true);
        }
        /// <summary>
        /// 输出到Exception目录
        /// </summary>
        public static void WriteErrorWithCategory(string category, object message)
        {
            SendMessage(LogLevel.ERROR, category, message?.ToString(), true);
        }
        /// <summary>
        /// 输出到Exception目录
        /// </summary>
        public static void WriteErrorFormat(string message, params object[] args)
        {
            SendMessage(LogLevel.ERROR, CATEGORY, Format(message, args), true);
        }
        /// <summary>
        /// 输出到Exception目录
        /// </summary>
        public static void WriteErrorFormatWithCategory(string category, string message, params object[] args)
        {
            SendMessage(LogLevel.ERROR, category, Format(message, args), true);
        }
        /// <summary>
        /// 输出到Exception目录
        /// </summary>
        public static void WriteException(Exception exception)
        {
            SendMessage(LogLevel.ERROR, CATEGORY, StringFactory.Make(exception), false);
        }

        /// <summary>
        /// 输出到Exception目录
        /// </summary>
        public static void WriteException(string message, Exception exception)
        {
            SendMessage(LogLevel.ERROR, CATEGORY, StringFactory.Make(message, exception), false);
        }
        /// <summary>
        /// 输出到Exception目录
        /// </summary>
        public static void WriteExceptionWithCategory(string category, Exception exception)
        {
            SendMessage(LogLevel.ERROR, category, StringFactory.Make(exception), false);
        }

        /// <summary>
        /// 输出到Exception目录
        /// </summary>
        public static void WriteExceptionWithCategory(string category, string message, Exception exception)
        {
            SendMessage(LogLevel.ERROR, category, StringFactory.Make(message, exception), false);
        }

        /// <summary>
        /// 输出到指定类别
        /// </summary>
        /// <param name="level">日志等级</param>
        /// <param name="category">日志类别</param>
        /// <param name="message">日志内容</param>
        public static void WriteTo(string level, string category, string message)
        {
            SendMessage(level, category, message, true);
        }

        /// <summary>
        /// 输出到指定类别
        /// </summary>
        /// <param name="level">日志等级</param>
        /// <param name="category">日志类别</param>
        /// <param name="message">日志内容</param>
        /// <param name="args"></param>
        public static void WriteTo(string level, string category, string message, params object[] args)
        {
            SendMessage(level, category, Format(message, args), true);
        }

        private static void SendMessage(string level, string category, string message, bool track)
        {
            message = FormatCustomMessage(category, message);
            if (LogStackTracker && track && LogStackLevels.Contains(level))
            {
                const string stackIndent = "  ";
                StackTrace stackTrace = new StackTrace(true);
                StringBuilder builder = new StringBuilder();
                int count = stackTrace.FrameCount;
                for (int i = 2; i < count; i++)
                {
                    StackFrame stackFrame = stackTrace.GetFrame(i);
                    MethodBase method = stackFrame.GetMethod();
                    string className = method.DeclaringType != null ? method.DeclaringType.FullName : "Unknow type";
                    string fileName = stackFrame.GetFileName();
                    builder.AppendFormat("{0} at {1}.{2}", stackIndent, className, method);
                    if (!string.IsNullOrEmpty(fileName))
                    {
                        builder.AppendFormat(" file {0}:line {1}", fileName, stackFrame.GetFileLineNumber());
                    }
                    builder.AppendLine();
                }
                message += "\r\n" + builder.ToString();
            }
            SendMessage(level, message);
        }

        /// <summary>
        /// 用输出格式格式化自定义数据
        /// </summary>
        /// <returns></returns>
        public static string FormatCustomMessage(string category, string message)
        {
            return $"{TimeLord.Now.ToString()} [{category}] - " + message;
        }

        ///// <summary>
        ///// 服务器内部输出
        ///// </summary>
        //public static void WriteLine(string level, string category, string message)
        //{
        //    message = FormatCustomMessage(category, message);
        //    WriteLine(level, message);
        //}
        ///// <summary>
        ///// 服务器内部输出
        ///// </summary>
        //public static void WriteLine(string level, string message)
        //{
        //    SendMessage(level, message);
        //}

        private static void SendMessage(string level, string message)
        {
            message += "\r\n";
            foreach (ILogger logger in loggers)
            {
                logger.SaveLog(level, message);
            }
        }

        private static string Format(string message, params object[] args)
        {
            return string.IsNullOrEmpty(message) ? "NULL" : string.Format(message, args);
        }
    }
}
