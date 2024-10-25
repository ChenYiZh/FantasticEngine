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
using System;
using System.Threading;
using System.Collections.Generic;
using System.Text;
using FantasyEngine.Collections;
using System.Threading.Tasks;
using FantasyEngine.Log;

namespace FantasyEngine.Timer
{
    /// <summary>
    /// 时间任务管理
    /// </summary>
    public sealed class TimeWorker
    {
        /// <summary>
        /// 计时器
        /// </summary>
        public System.Threading.Timer Timer { get; private set; }
        /// <summary>
        /// 时间计划
        /// </summary>
        private ThreadSafeDictionary<string, TimeSchedule> _schedules;
        /// <summary>
        /// 初始化
        /// </summary>
        public TimeWorker()
        {
            _schedules = new ThreadSafeDictionary<string, TimeSchedule>();
        }
        /// <summary>
        /// 计时器间隔
        /// </summary>
        private const int INTERVAL = 100;
        /// <summary>
        /// 重启
        /// </summary>
        public void Restart()
        {
            Stop();
            Timer = new System.Threading.Timer(Update, null, 0, INTERVAL);
        }
        /// <summary>
        /// 启动
        /// </summary>
        public void Start()
        {
            if (Timer != null)
            {
                Stop();
            }
            Timer = new System.Threading.Timer(Update, null, 0, INTERVAL);
        }
        /// <summary>
        /// 计时器执行的操作
        /// </summary>
        private void Update(object state)
        {
            List<string> removeCache = new List<string>();
            foreach (KeyValuePair<string, TimeSchedule> schedule in _schedules)
            {
                schedule.Value.Execute();
                if (schedule.Value.IsExpired)
                {
                    removeCache.Add(schedule.Key);
                }
            };
            foreach (string key in removeCache)
            {
                _schedules.Remove(key);
            }
        }
        /// <summary>
        /// 添加新的时间计划
        /// </summary>
        public void Append(TimeSchedule schedule)
        {
            if (schedule == null)
            {
                FConsole.WriteError("TimeSchedule is Null! ");
                return;
            }
            if (_schedules.ContainsKey(schedule.Name))
            {
                FConsole.WriteError("There is a same name of schedule.: " + schedule.Name);
                return;
            }
            lock (_schedules)
            {
                _schedules.Add(schedule.Name, schedule);
            }
        }
        /// <summary>
        /// 移除时间计划
        /// </summary>
        public void Remove(TimeSchedule schedule)
        {
            if (schedule == null)
            {
                return;
            }
            string key = null;
            foreach (KeyValuePair<string, TimeSchedule> plan in _schedules)
            {
                if (plan.Value == schedule)
                {
                    key = plan.Key;
                }
            }
            // WARN: 这里会出现集合修改的问题
            lock (_schedules)
            {
                if (key == null && _schedules.ContainsKey(schedule.Name))
                {
                    _schedules.Remove(schedule.Name);
                    return;
                }
                _schedules.Remove(key);
            }
        }
        /// <summary>
        /// 移除并结束所有任务
        /// </summary>
        public void Stop()
        {
            _schedules.Clear();
            if (Timer != null)
            {
                Timer.Dispose();
                Timer = null;
            }
        }
    }
}
