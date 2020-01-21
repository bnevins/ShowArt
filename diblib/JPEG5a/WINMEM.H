// WBN 7/95

extern BYTE huge *		AllocWinMem(unsigned long);
extern unsigned long	GetNumDisplayColors();
extern void				win_progress_monitor(int numPass, int numPasses, int percent);
extern void 			DestroyProgressWindow();
extern void 			CreateProgressWindow(const char* caption);
