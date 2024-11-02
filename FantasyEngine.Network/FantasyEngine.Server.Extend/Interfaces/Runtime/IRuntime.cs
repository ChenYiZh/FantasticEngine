/****************************************************************************
THIS FILE IS PART OF Fantasy Engine PROJECT
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

namespace FantasyEngine.Network.Server
{
    /// <summary>
    /// 自定义运行时
    /// </summary>
    public interface IRuntime
    {
        /// <summary>
        /// 启动，但未启动服务器时执行
        /// </summary>
        void OnStartup();
        /// <summary>
        /// 在数据库管理对象初始化完成时执行，
        /// 并没有连接，也没有数据
        /// </summary>
        void OnDatebaseInitialized();
        /// <summary>
        /// 准备开始启动服务器时调用，
        /// 数据库已经连接，并且热数据已经载入
        /// </summary>
        void ReadyToStartServers();
        /// <summary>
        /// 服务器启动后执行
        /// </summary>
        void OnBegun();
        /// <summary>
        /// 在关闭前执行
        /// </summary>
        void OnShutdown();
        /// <summary>
        /// 在退出时执行
        /// </summary>
        void OnKilled();
    }
}
