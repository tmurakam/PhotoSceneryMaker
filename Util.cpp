//---------------------------------------------------------------------------

#define NO_WIN32_LEAN_AND_MEAN

#include <vcl.h>
#pragma hdrstop

#include <shlobj.h>

#include "Util.h"

//---------------------------------------------------------------------------

//#pragma package(smart_init)

static int CALLBACK BrowseCallbackProc(HWND hwnd,UINT uMsg,LPARAM lParam,LPARAM lpData)
{
    if(uMsg==BFFM_INITIALIZED){
	SendMessage(hwnd,BFFM_SETSELECTION,(WPARAM)TRUE,lpData);
    }
    return 0;
}


AnsiString SelectFolder(
	AnsiString InitFolder,	// 初期フォルダ
	HWND owner)              //オーナーウインドウのハンドル
{
  AnsiString s;
  IMalloc *m; //タスクアロケータ

  if(SUCCEEDED(SHGetMalloc(&m)))
  {
    BROWSEINFO b;
    char path[MAX_PATH];

    b.hwndOwner= owner; //オーナーウィンドウ
    b.pidlRoot= NULL;   //ルート
    b.pszDisplayName= path; //選択したものの表示名格納場所
    b.lpszTitle= "フォルダを選んで下さい"; //表示メッセージ
    b.ulFlags= BIF_RETURNONLYFSDIRS; //フォルダのみ選べる
    b.lpfn= BrowseCallbackProc; //コールバック関数
    b.lParam= (LPARAM)InitFolder.c_str();  //コールバック関数に渡される値
    b.iImage= 0;  //選択したもののイメージ番号
  //ダイアログの表示
    LPITEMIDLIST id=SHBrowseForFolder(&b);
    if(id)
    {
    //パス名を得る
      SHGetPathFromIDList(id, path);
      s= path;
      m->Free(id);
    }
    m->Release();
  }
  return s;
}
