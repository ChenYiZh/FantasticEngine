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
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
namespace FantasyEngine
{
    /// <summary>
    /// BGM 管理可以产生一个过度效果，
    /// 必须依赖AudioSystem
    /// </summary>
    public partial class MusicSystem : SystemBasis<MusicSystem>
    {
        /// <summary>
        /// 是否正在音乐过渡
        /// </summary>
        private bool _working = false;
        /// <summary>
        /// 音量扬升
        /// </summary>
        private bool _up = false;
        /// <summary>
        /// 下一段音乐
        /// </summary>
        private AudioClip _nextMusic = null;
        /// <summary>
        /// 过渡时长
        /// </summary>
        private const float COMBINE_TIME = 3;
        /// <summary>
        /// 是否打断当前BGM直接播放
        /// </summary>
        /// <param name="music"></param>
        /// <param name="interrupt"></param>
        public void PlayMusic(AudioClip music, bool interrupt = false)
        {
            if (AudioSystem.Instance.MusicSource.clip == music)
            {
                AudioSystem.Instance.MusicSource.volume = AudioSystem.Instance.MusicVolume;
                return;
            }
            if (interrupt)
            {
                _working = false;
                AudioSystem.Instance.PlayMusic(music);
            }
            else
            {
                _nextMusic = music;
                _working = true;
                _up = AudioSystem.Instance.MusicSource.clip == null ? true : false;
            }
        }

        public override void OnUpdate()
        {
            base.OnUpdate();
            if (_working && _working)
            {
                if (!_up)
                {
                    float volume = AudioSystem.Instance.MusicSource.volume - Time.deltaTime / COMBINE_TIME;
                    AudioSystem.Instance.MusicSource.volume = Mathf.Clamp01(volume);
                    if (volume < 0)
                    {
                        _up = true;
                    }
                }
                else
                {
                    if (_nextMusic != null)
                    {
                        AudioSystem.Instance.PlayMusic(_nextMusic);
                        _nextMusic = null;
                        AudioSystem.Instance.MusicSource.volume = 0;
                    }
                    float volume = AudioSystem.Instance.MusicSource.volume + Time.deltaTime / COMBINE_TIME;
                    AudioSystem.Instance.MusicSource.volume = Mathf.Clamp01(volume);
                    if (volume >= AudioSystem.Instance.MusicVolume)
                    {
                        AudioSystem.Instance.MusicSource.volume = AudioSystem.Instance.MusicVolume;
                        _working = false;
                    }
                }
            }
        }

        public override void OnExit()
        {
            base.OnExit();
            _working = true;
            AudioSystem.Instance.MusicSource.volume = 0;
            _nextMusic = AudioSystem.Instance.MusicSource.clip;
            AudioSystem.Instance.StopMusic();
        }
    }
}