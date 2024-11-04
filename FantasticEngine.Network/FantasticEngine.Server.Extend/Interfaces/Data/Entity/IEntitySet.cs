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
using System;
using System.Collections.Generic;
using System.Text;

namespace FantasticEngine.Network.Server.Entity
{
    /// <summary>
    /// 数据对象池，
    /// 默认只加载缓存中的数据。
    /// 如果需要针对完整数据进行遍历，先执行LoadAll
    /// </summary>
    /// <typeparam name="T"></typeparam>
    public interface IEntitySet<T> : IEnumerable<T> where T : MajorEntity, new()
    {
        ///// <summary>
        ///// 查找，如果缓存中找不到，会从数据库中查询
        ///// </summary>
        //T Find(long entityId);

        /// <summary>
        /// 主键查找，如果缓存中找不到，会从数据库中查询
        /// </summary>
        T Find(params object[] keys);

        /// <summary>
        /// 主键类查询
        /// </summary>
        T Find(EntityKey key);

        /// <summary>
        /// 根据Lamda返回新的列表，不会影响内部数据列表
        /// </summary>
        IList<T> Find(Func<T, bool> condition);

        /// <summary>
        /// 添加数据
        /// </summary>
        bool AddOrUpdate(T entity);

        /// <summary>
        /// 删除数据，同时删除缓存，Redis，数据库中的数据
        /// </summary>
        bool Remove(T entity);

        /// <summary>
        /// 通过Key删除数据，同时删除缓存，Redis，数据库中的数据
        /// </summary>
        bool Remove(EntityKey key);

        /// <summary>
        /// 加载完整的数据，从数据库中加载并附加到缓存中
        /// </summary>
        void LoadAll();
    }
}
