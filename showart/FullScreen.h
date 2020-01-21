// FullScreen.h: interface for the FullScreen class.
//
//////////////////////////////////////////////////////////////////////
#ifndef FULLSCREEN__H__
#define FULLSCREEN__H__

#if _MSC_VER < 1001
#error MSC Version is too old
#endif

#pragma once
class CMainFrame;

class FullScreen  
{
public:
	void	stopFullScreen(CMainFrame* pFrame);
	void	startFullScreen(CMainFrame* pFrame);
	bool	isFullScreen();
			FullScreen();
private:
	bool wasZoomed;
	bool	isForcedMaximized;
	bool	isFullScreenMode;
	bool	toolbarWasVisible;
	bool	statusbarWasVisible;
};
#endif

