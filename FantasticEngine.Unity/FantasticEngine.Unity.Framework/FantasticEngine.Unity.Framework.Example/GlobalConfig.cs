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
using FantasticEngine.UI;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace FantasticEngine
{
    /// <summary>
    /// 创建许全季管理
    /// </summary>
    public class GlobalConfig
    {
        public static void RegistSystem(HashSet<SystemBasis> systems)
        {
            systems.Add(Logger.Instance);
            systems.Add(LoggerToast.Instance);
            systems.Add(LogFile.Instance);
            systems.Add(PlayerData.Instance);
            systems.Add(EventSystem.Instance);

            systems.Add(TableManager.Instance);

            systems.Add(WebSystem.Instance);

            systems.Add(UIFactory.Instance);
            systems.Add(SceneSystem.Instance);

            systems.Add(AudioSystem.Instance);
            systems.Add(InputSystem.Instance);

            systems.Add(TextureSystem.Instance);
        }

        public static void RegistPanels(UIFactory factory)
        {
            /**
             * Exmaple: factory.Regist<UI_Main>(EUIID.UI_Main, "UI_Main", EUILevel.Bottom, false, EUIAnim.None, 1f, EUIAnim.None);
             * **/
        }
    }
}