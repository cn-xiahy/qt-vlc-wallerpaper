#include "wallerpaper.h"
#include "windows.h"
#include <QFileDialog>
#include <iostream>

#include "vlc/vlc.h"


#define ssize_t SSIZE_T
QString fileName;
volatile int s = 0;
HWND hwnd_progman;
HWND hDefView;
HWND workerw;
HWND progman;
libvlc_media_player_t* mp;

wallerpaper::wallerpaper(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	connect(ui.pushButton, &QPushButton::clicked, this, &wallerpaper::pushbutton);
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM Lparam)
{
     hDefView = FindWindowEx(hwnd, 0, L"SHELLDLL_DefView", 0);
    if (hDefView != 0) {
         workerw = FindWindowEx(0, hwnd, L"WorkerW", 0);
		 ShowWindow(workerw, SW_HIDE);
		 return 0;
    }
    return 1;
}

int wallerpaper::choose()
{
	hwnd_progman = FindWindow(L"Progman", NULL);
	
	if(s==1)
	{
		libvlc_media_player_stop(mp);
	}
	
	if (hwnd_progman == NULL)
	{
		return 1;
	}
	//DWORD_PTR result = 0;
	SendMessageTimeout(hwnd_progman, 0x052C, NULL, NULL, NULL, 100, NULL); \

		EnumWindows(EnumWindowsProc, NULL);

	const auto path = QDir::toNativeSeparators(fileName);
	const std::string stdStringPath = path.toStdString();
	const char* p = stdStringPath.c_str();
	libvlc_instance_t* inst = libvlc_new(0, NULL);
	libvlc_media_t* m = libvlc_media_new_path(inst, p);

	libvlc_media_list_t* ml = libvlc_media_list_new(inst);
	mp = libvlc_media_player_new_from_media(m);
	libvlc_media_list_player_t* mlp = libvlc_media_list_player_new(inst);

	libvlc_media_list_add_media(ml, m);
	libvlc_media_release(m);

	libvlc_media_list_player_set_media_list(mlp, ml);
	libvlc_media_list_player_set_media_player(mlp, mp);
	libvlc_media_list_player_set_playback_mode(mlp,libvlc_playback_mode_loop);
	libvlc_media_player_set_hwnd(mp, hwnd_progman);
	libvlc_audio_set_volume(mp, 100);
	libvlc_media_list_player_play(mlp);
	return 1;
}



void wallerpaper::pushbutton()
{
	fileName = QFileDialog::getOpenFileName
        (
            this,
            tr("文件对话框！"),
            "",
            tr( "(*.*)")
         );
	
	choose();
	
	if (fileName != NULL)
	{
		s = 1;
	}
	
    return;
}

