/****************************************************************************
THIS FILE IS PART OF Fantastic Engine PROJECT
THIS PROGRAM IS FREE SOFTWARE, IS LICENSED UNDER MIT

Copyright (c) 2022-2030 ChenYiZh
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
using FantasticEngine.Network.Server.Entity;
using System;
using System.Collections.Generic;
using System.Text;

namespace FantasticEngine.Network.Server
{
    /// <summary>
    /// 缓存中的数据结构
    /// </summary>
    internal interface IDbSet
    {
        /// <summary>
        /// 表结构配置
        /// </summary>
        ITableScheme TableScheme { get; }
        /// <summary>
        /// 用于锁的对象
        /// </summary>
        object SyncRoot { get; }
        /// <summary>
        /// 卸载冷数据倒计时
        /// </summary>
        int ReleaseCountdown { get; set; }
        /// <summary>
        /// 推送倒计时
        /// </summary>
        int CommitCountdown { get; set; }
        /// <summary>
        /// 卸载冷数据
        /// </summary>
        void ReleaseColdEntities();
        /// <summary>
        /// 将修改过的数据进行提交
        /// </summary>
        void CommitModifiedData();
        /// <summary>
        /// 将所有修改过的数据进行提交
        /// </summary>
        void ForceCommitAllModifiedData();
        /// <summary>
        /// 非泛型修改数据通知，内部强制转换
        /// </summary>
        void OnModified(EntityKey key, MajorEntity entity);
        /// <summary>
        /// 重新拉取热数据
        /// </summary>
        void PullAllRawData();
        /// <summary>
        /// 将所有的缓存的热数据全部推送出去
        /// </summary>
        void PushAllRawData();
        /// <summary>
        /// 移出冷数据
        /// </summary>
        void CheckOutColdEntities();
        /// <summary>
        /// 关闭
        /// </summary>
        void Release();
    }

    /// <summary>
    /// 缓存中的数据结构
    /// </summary>
    internal interface IDbSet<T> : IDbSet where T : MajorEntity, new()
    {
        /// <summary>
        /// 数据变化监听
        /// </summary>
        event OnDbSetDataModified<T> OnDataModified;
        /// <summary>
        /// 缓存中的数据，热数据
        /// </summary>
        IReadOnlyDictionary<EntityKey, T> RawEntities { get; }
        /// <summary>
        /// 现在真在使用的修改缓存池id
        /// </summary>
        int ModifiedEntitiesPoolIndex { get; }
        /// <summary>
        /// 已经改动过的实例，这些数据应该会出现在热数据中，主要用于推送。
        /// 防止多线程阻塞，所以使用集合池
        /// </summary>
        IReadOnlyList<IReadOnlyDictionary<EntityKey, T>> ModifiedEntitiesPool { get; }
        /// <summary>
        /// 冷数据
        /// </summary>
        IReadOnlyDictionary<EntityKey, T> ColdEntities { get; }
        /// <summary>
        /// 修改数据通知
        /// </summary>
        void OnModified(EntityKey key, T entity);
        /// <summary>
        /// 推入所有数据
        /// </summary>
        /// <returns></returns>
        IReadOnlyDictionary<EntityKey, T> LoadAll();
        /// <summary>
        /// 冷热数据库查询
        /// </summary>
        T Find(EntityKey key);
    }
}
