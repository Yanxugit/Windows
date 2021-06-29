# Windows
windows 程序设置   第五版   C++

编译问提
error LNK2019: 无法解析的外部符号 __imp__PlaySoundW@12，该符号在函数 "long __stdcall WndPorc(struct HWND__ *,unsigned int,unsigned int,long)" (?WndPorc@@YGJPAUHWND__@@IIJ@Z) 中被引用

解决：
项目-》属性-》连接器-》输入-》附加依赖项-》编辑 —-》加入  Winmm.lib
