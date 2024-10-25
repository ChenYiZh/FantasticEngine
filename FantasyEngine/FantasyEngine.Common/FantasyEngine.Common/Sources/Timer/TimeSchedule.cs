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
using FantasyEngine.Log;
using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;

namespace FantasyEngine.Timer
{
    /// <summary>
    /// 计划周期
    /// </summary>
    public enum PlanCycle
    {
        /// <summary>
        /// 
        /// </summary>
        Once,
        /// <summary>
        /// 
        /// </summary>
        Day,
        /// <summary>
        /// 
        /// </summary>
        Week,
    }
    /// <summary>
    /// 时间计划回调
    /// </summary>
    public delegate void TimeScheduleCallback(TimeSchedule planConfig);
    /// <summary>
    /// 时间计划
    /// </summary>
    public class TimeSchedule
    {
        /// <summary>
        /// 执行次数
        /// </summary>
        public int ExcutingNum { get; private set; }
        /// <summary>
        /// 是否循环
        /// </summary>
        public bool IsCycle { get; private set; }
        /// <summary>
        /// 名称
        /// </summary>
        public string Name { get; private set; }
        /// <summary>
        /// 回调
        /// </summary>
        public TimeScheduleCallback Callback { get; private set; }
        /// <summary>
        /// 开始时间
        /// </summary>
        public string BeginTime { get; private set; }
        /// <summary>
        /// 执行时长
        /// </summary>
        public int DeltaSeconds { get; private set; }
        /// <summary>
        /// 执行间隔
        /// </summary>
        public int SecondInterval { get; private set; }
        /// <summary>
        /// 最新执行时间
        /// </summary>
        public DateTime ExecTime { get; private set; }
        /// <summary>
        /// 下次执行时间
        /// </summary>
        public DateTime NextTime { get; private set; }
        /// <summary>
        /// 循环周期
        /// </summary>
        public PlanCycle PlanCycle { get; private set; }
        /// <summary>
        /// 执行星期
        /// </summary>
        public DayOfWeek DayOfWeek { get; private set; }
        /// <summary>
        /// 是否到期
        /// </summary>
        public bool IsExpired { get; private set; }
        /// <summary>
        /// 是否一直在执行阶段
        /// </summary>
        public bool IsExecuting { get; private set; }
        /// <summary>
        /// 结束时间
        /// </summary>
        private DateTime EndTime { get; set; }
        /// <summary>
        /// 时间计划
        /// </summary>
        /// <param name="isCycle">是否循环</param>
        /// <param name="planCycle">循环类型</param>
        /// <param name="name">唯一名称</param>
        /// <param name="callback">回调</param>
        /// <param name="beginTime">开始时间"2015/01/01 09:00:00", 或者 "09:00:00"</param>
        /// <param name="endTime">结束时间"2015/01/01 09:00:00", 或者 "09:00:00"</param>
        /// <param name="secondInterval">间隔</param>
        /// <param name="week">周几执行</param>
        public TimeSchedule(bool isCycle, PlanCycle planCycle, string name, TimeScheduleCallback callback, string beginTime, string endTime = null, int secondInterval = 0, DayOfWeek week = DayOfWeek.Sunday)
        {
            ExcutingNum = 0;
            IsExecuting = false;
            IsExpired = false;
            DeltaSeconds = 0;
            SecondInterval = 0;

            IsCycle = isCycle;
            PlanCycle = planCycle;
            Name = name;
            Callback = callback;
            BeginTime = beginTime;
            if (endTime != null)
            {
                DeltaSeconds = (int)(Convert.ToDateTime(endTime) - Convert.ToDateTime(beginTime)).TotalSeconds;
            }
            SecondInterval = secondInterval;
            DayOfWeek = week;

            Reset();
        }
        /// <summary>
        /// 执行任务
        /// </summary>
        /// <returns></returns>
        internal bool Execute()
        {
            if (IsTrigger())
            {
                ThreadPool.QueueUserWorkItem((plan) =>
                {
                    try
                    {
                        FConsole.WriteFormatWithCategory(Categories.TIME_LORD + ": " + Name, "Timer Execute: {0}", Name);
                        Callback(this);
                    }
                    catch (Exception e)
                    {
                        FConsole.WriteExceptionWithCategory(Categories.TIME_LORD + ": " + Name, e);
                    }
                    finally
                    {
                        ExcutingNum++;
                    }
                }, this);
                return true;
            }
            return false;
        }
        private void Reset()
        {
            ExcutingNum = 0;
            IsExpired = false;
            DateTime beginTime = Convert.ToDateTime(BeginTime);

            switch (PlanCycle)
            {
                case PlanCycle.Day:
                    if (beginTime < Current)
                    {
                        NextTime = new DateTime(beginTime.Ticks - beginTime.Date.Ticks + Current.Date.Ticks);
                        if (SecondInterval > 0)
                        {
                            int deltaRate = (int)((Current - NextTime).TotalSeconds / SecondInterval) + 1;
                            if (NextTime < Current)
                            {
                                NextTime = NextTime.AddSeconds(deltaRate * SecondInterval);
                            }
                        }
                        else
                        {
                            if (NextTime < Current)
                            {
                                NextTime = NextTime.AddDays(1);
                            }
                        }
                    }
                    else
                    {
                        NextTime = beginTime;
                    }
                    break;
                case PlanCycle.Week:
                    if (Current.DayOfWeek != DayOfWeek || beginTime < Current)
                    {
                        NextTime = new DateTime(beginTime.Ticks - beginTime.Date.Ticks + Current.Date.Ticks);
                        if (NextTime < Current || NextTime.DayOfWeek != DayOfWeek)
                        {
                            int dayOfWeek = ((int)NextTime.DayOfWeek + 7) % 7;
                            int deltaDay = ((int)DayOfWeek + 7 - dayOfWeek) % 7;
                            if (deltaDay == 0)
                            {
                                deltaDay = 7;
                            }
                            NextTime = NextTime.AddDays(deltaDay);
                        }
                    }
                    else
                    {
                        NextTime = beginTime;
                    }
                    break;
                default:
                    if (beginTime < Current)
                    {
                        IsExpired = true;
                    }
                    else
                    {
                        NextTime = beginTime;
                    }
                    break;
            }
        }
        private bool IsTrigger()
        {
            if (IsExpired)
            {
                return false;
            }
            else if (IsExecuting)
            {
                if (Current <= EndTime && Convert.ToDateTime(ExecTime.ToString("yyyy/MM/dd HH:mm:ss")).AddSeconds(SecondInterval) <= Current)
                {
                    ExecTime = Current;
                    return true;
                }
                else
                {
                    IsExecuting = Current <= EndTime;
                    return false;
                }
            }
            else if (NextTime < Current)
            {
                ExecTime = Current;
                switch (PlanCycle)
                {
                    case PlanCycle.Once:
                        IsExpired = true;
                        break;
                    case PlanCycle.Day:
                        if (SecondInterval > 0 && Current.Date == NextTime.Date)
                        {
                            int deltaRate = (int)((Current - NextTime).TotalSeconds / SecondInterval) + 1;
                            NextTime = NextTime.AddSeconds(deltaRate * SecondInterval);
                        }
                        else
                        {
                            if (Current.Date == NextTime.Date)
                            {
                                NextTime = NextTime.AddDays(1);
                            }
                            else
                            {
                                DateTime beginTime = Convert.ToDateTime(BeginTime);
                                NextTime = new DateTime(beginTime.Ticks - beginTime.Date.Ticks + Current.Date.Ticks);
                            }
                        }
                        break;
                    case PlanCycle.Week:
                        NextTime = NextTime.AddDays(7);
                        break;
                }
                if (DeltaSeconds > 0 && SecondInterval > 0)
                {
                    IsExecuting = true;
                }
                return true;
            }
            return false;
        }
        private DateTime Current
        {
            get { return TimeLord.Now; }
        }
        /// <summary>
        /// 创建每日任务
        /// </summary>
        /// <param name="callback">任务回调</param>
        /// <param name="name">唯一名称</param>
        /// <param name="beginTime">开始时间"2015/01/01 09:00:00", 或者 "09:00:00"</param>
        public static TimeSchedule EveryDayPlan(TimeScheduleCallback callback, string name, string beginTime)
        {
            return new TimeSchedule(true, PlanCycle.Day, name, callback, beginTime);
        }

        /// <summary>
        /// 每分钟任务
        /// </summary>
        /// <param name="callback">任务回调</param>
        /// <param name="name">唯一名称</param>
        /// <param name="beginTime">开始时间"2015/01/01 09:00:00", 或者 "09:00:00"</param>
        /// <param name="endTime">结束时间"2015/01/01 09:00:00", 或者 "09:00:00"</param>
        /// <param name="secondInterval">循环间隔</param>
        public static TimeSchedule EveryMinutePlan(TimeScheduleCallback callback, string name, string beginTime, string endTime, int secondInterval = 60)
        {
            return new TimeSchedule(true, PlanCycle.Day, name, callback, beginTime, endTime, secondInterval);
        }

        /// <summary>
        /// 每周任务
        /// </summary>
        /// <param name="callback">任务回调</param>
        /// <param name="name">唯一名称</param>
        /// <param name="week">周几执行</param>
        /// <param name="beginTime">开始时间"09:00:00"</param>
        /// <returns></returns>
        public static TimeSchedule EveryWeekPlan(TimeScheduleCallback callback, string name, DayOfWeek week, string beginTime)
        {
            return new TimeSchedule(true, PlanCycle.Week, name, callback, beginTime, null, 0, week);
        }

        /// <summary>
        /// 每周任务
        /// </summary>
        /// <param name="callback">任务回调</param>
        /// <param name="name">唯一名称</param>
        /// <param name="week">周几执行</param>
        /// <param name="beginTime">开始时间"09:00:00"</param>
        /// <param name="endTime">结束时间"09:00:00"</param>
        /// <param name="secondInterval">循环间隔</param>
        public static TimeSchedule EveryWeekPlan(TimeScheduleCallback callback, string name, DayOfWeek week, string beginTime, string endTime, int secondInterval = 60)
        {
            return new TimeSchedule(true, PlanCycle.Week, name, callback, beginTime, endTime, secondInterval, week);
        }

        /// <summary>
        /// 只执行一次
        /// </summary>
        /// <param name="callback">任务回调</param>
        /// <param name="name">唯一名称</param>
        /// <param name="beginTime">开始时间"2015/01/01 09:00:00", 或者 "09:00:00"</param>
        public static TimeSchedule OncePlan(TimeScheduleCallback callback, string name, string beginTime)
        {
            return new TimeSchedule(false, PlanCycle.Once, name, callback, beginTime);
        }
    }
}
