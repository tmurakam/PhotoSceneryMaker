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
	AnsiString InitFolder,	// �����t�H���_
	HWND owner)              //�I�[�i�[�E�C���h�E�̃n���h��
{
  AnsiString s;
  IMalloc *m; //�^�X�N�A���P�[�^

  if(SUCCEEDED(SHGetMalloc(&m)))
  {
    BROWSEINFO b;
    char path[MAX_PATH];

    b.hwndOwner= owner; //�I�[�i�[�E�B���h�E
    b.pidlRoot= NULL;   //���[�g
    b.pszDisplayName= path; //�I���������̂̕\�����i�[�ꏊ
    b.lpszTitle= "�t�H���_��I��ŉ�����"; //�\�����b�Z�[�W
    b.ulFlags= BIF_RETURNONLYFSDIRS; //�t�H���_�̂ݑI�ׂ�
    b.lpfn= BrowseCallbackProc; //�R�[���o�b�N�֐�
    b.lParam= (LPARAM)InitFolder.c_str();  //�R�[���o�b�N�֐��ɓn�����l
    b.iImage= 0;  //�I���������̂̃C���[�W�ԍ�
  //�_�C�A���O�̕\��
    LPITEMIDLIST id=SHBrowseForFolder(&b);
    if(id)
    {
    //�p�X���𓾂�
      SHGetPathFromIDList(id, path);
      s= path;
      m->Free(id);
    }
    m->Release();
  }
  return s;
}
