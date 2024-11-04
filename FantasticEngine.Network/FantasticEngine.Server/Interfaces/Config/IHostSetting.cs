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

namespace FantasticEngine.Network
{
    /// <summary>
    /// 服务器配置
    /// </summary>
    public interface IHostSetting
    {
        /// <summary>
        /// 服务器标识
        /// </summary>
        string Name { get; }

        /// <summary>
        /// 端口号
        /// </summary>
        int Port { get; }

        /// <summary>
        /// 类型
        /// </summary>
        ESocketType Type { get; }

        /// <summary>
        /// 自定义脚本的完整名称，可以不设置，使用原生的管理类
        /// </summary>
        string ClassFullname { get; }

        /// <summary>
        /// 消息处理的完整类名，用{0}嵌入id
        /// </summary>
        string ActionClassFullName { get; }

        /// <summary>
        /// 执行类
        /// </summary>
        string MainClass { get; }

        /// <summary>
        /// TCP全连接队列长度
        /// </summary>
        int Backlog { get; }

        /// <summary>
        /// 最大并发数量
        /// </summary>
        int MaxConnections { get; }

        /// <summary>
        /// 默认连接对象池容量
        /// </summary>
        int MaxAcceptCapacity { get; }

        /// <summary>
        /// 默认消息处理连接池容量大小
        /// </summary>
        int MaxIOCapacity { get; }

        /// <summary>
        /// 数据通讯缓存字节大小
        /// </summary>
        int BufferSize { get; }

        /// <summary>
        /// 通讯内容整体偏移
        /// </summary>
        int Offset { get; }

        /// <summary>
        /// 是否使用压缩
        /// </summary>
        bool UseGZip { get; }

        /// <summary>
        /// 获取类别显示
        /// </summary>
        string GetCategory();
    }
}
