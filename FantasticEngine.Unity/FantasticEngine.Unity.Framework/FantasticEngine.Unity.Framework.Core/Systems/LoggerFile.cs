/****************************************************************************
THIS FILE IS PART OF Fantastic Engine PROJECT
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
using System;
using System.Collections;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.IO;
using UnityEngine;
namespace FantasticEngine
{
    /// <summary>
    /// 输出日志到文件
    /// </summary>
    public class LogFile : SystemBasis<LogFile>, Log.ILogger
    {
        private ConcurrentQueue<KeyValuePair<string, string>>
            Messages = new ConcurrentQueue<KeyValuePair<string, string>>();

        private readonly string _filePath = Path.Combine(Application.persistentDataPath, $"console_{DateTime.Now.ToString("yyyyMMddHHmmss")}.log");

        public void SaveLog(string level, string message)
        {
            Messages.Enqueue(new KeyValuePair<string, string>(level, message));
        }

        private void Log(string level, string message)
        {
            System.IO.File.AppendAllText(_filePath, message);
        }

        public override void OnUpdate()
        {
            base.OnUpdate();
            KeyValuePair<string, string> message;
            while (Messages.Count > 0 && Messages.TryDequeue(out message))
            {
                Log(message.Key, message.Value);
            }
        }
    }
}