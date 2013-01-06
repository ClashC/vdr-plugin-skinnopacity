#ifndef __NOPACITY_MENUITEM_H
#define __NOPACITY_MENUITEM_H

class cNopacityMenuItem : public cListObject, public cThread {
protected:
    cOsd *osd;
    cPixmap *pixmap;
    cPixmap *pixmapIcon;
    cPixmap *pixmapTextScroller;
    int *handleBackgrounds;
    const char *Text;
    bool selectable;
    bool current;
    bool wasCurrent;
    bool scrollable;
    bool drawn;
    cFont *font;
    cFont *fontSmall;
    cFont *fontEPGWindow;
    int width, height;
    int top, left;
    int textLeft;
    int index;
    cString *itemTabs;
    int *tabWidth;
    int numTabs;
    cRect *textWindow;
    cNopacityTextWindow *infoTextWindow;
    virtual void SetTextFull(void) {};
    virtual void SetTextShort(void) {};
    void DrawDelimiter(const char *del, const char *icon, int handleBgrd);
    virtual void Action(void);
    void DoSleep(int duration);
    std::string CutText(std::string *text, int width, const cFont *font);
public:
    cNopacityMenuItem(cOsd *osd, const char *text, bool sel);
    virtual ~cNopacityMenuItem(void);
    void CreatePixmap();
    void CreatePixmapIcon(void);
    virtual void CreatePixmapTextScroller(int totalWidth);
    void SetGeometry(int index, int top, int left, int width, int height);
    void SetFont(cFont *font) {this->font = font;}
    void SetFontSmall(cFont *fontSmall) {this->fontSmall = fontSmall;}
    void SetFontEPGWindow(cFont *font) {this->fontEPGWindow = font;}
    void SetCurrent(bool cur); 
    void SetAlpha(int alpha) {this->pixmap->SetAlpha(alpha);}
    void SetAlphaIcon(int alpha) {if (pixmapIcon) this->pixmapIcon->SetAlpha(alpha);}
    void SetAlphaText(int alpha) {if (pixmapTextScroller) this->pixmapTextScroller->SetAlpha(alpha);}
    void SetTabs(cString *tabs, int *tabWidths, int numtabs);
    void SetBackgrounds(int *handleBackgrounds);
    void SetTextWindow(cRect *window) {textWindow = window;};
    virtual void CreateText(void) {};
    virtual int CheckScrollable(bool hasIcon) {return 0;};
    virtual void Render() = 0;
};

class cNopacityMainMenuItem : public cNopacityMenuItem {
private:
    cString menuNumber;
    cString menuEntry;
    std::string strEntry;
    std::string strEntryFull;
    static std::string items[16];
    cString GetIconName();
    void SetTextFull(void);
    void SetTextShort(void);
public:
    cNopacityMainMenuItem(cOsd *osd, const char *text, bool sel);
    ~cNopacityMainMenuItem(void);
    void CreatePixmapTextScroller(int totalWidth);
    void CreateText(void);
    int CheckScrollable(bool hasIcon);
    void Render();
};

class cNopacityScheduleMenuItem : public cNopacityMenuItem {
private:
    eMenuCategory category;
    const cEvent *Event;
    const cChannel *Channel;
    eTimerMatch TimerMatch;
    std::string strDateTime;
    std::string strTitle;
    std::string strSubTitle;
    std::string strTitleFull;
    std::string strSubTitleFull;
    bool scrollTitle;
    bool scrollSubTitle;
    int titleY;
    void DrawBackground(int textLeft);
    void DrawLogo(int logoWidth, int logoHeight);
    void DrawRemaining(int x, int y, int width);
    void SetTextFull(void);
    void SetTextShort(void);
public:
    cNopacityScheduleMenuItem(cOsd *osd, const cEvent *Event, const cChannel *Channel, eTimerMatch TimerMatch, bool sel, eMenuCategory category);
    ~cNopacityScheduleMenuItem(void);
    void CreatePixmapTextScroller(int totalWidth);
    void CreateText(void);
    int CheckScrollable(bool hasIcon);
    void Render();
};

class cNopacityChannelMenuItem : public cNopacityMenuItem {
private:
    const cChannel *Channel;
    std::string strEntry;
    std::string strEntryFull;
    void SetTextFull(void);
    void SetTextShort(void);
public:
    cNopacityChannelMenuItem(cOsd *osd, const cChannel *Channel, bool sel);
    ~cNopacityChannelMenuItem(void);
    void CreatePixmapTextScroller(int totalWidth);
    void CreateText(void);
    int CheckScrollable(bool hasIcon);
    void Render();
};

class cNopacityRecordingMenuItem : public cNopacityMenuItem {
private:
    const cRecording *Recording;
    bool isFolder;
    int Level, Total, New;
    std::string strRecName;
    std::string strRecNameFull;
    void SetTextFull(void);
    void SetTextShort(void);
    void SetTextFullFolder(void);
    void SetTextShortFolder(void);
    void SetTextFullRecording(void);
    void SetTextShortRecording(void);
    int DrawRecordingNewIcon(void);
    int DrawRecordingEditedIcon(int startLeft);
    void DrawFolderIcon(void);
    void DrawRecDateTime(void);
    void DrawFolderNewSeen(void);
    int CheckScrollableRecording(void);
    int CheckScrollableFolder(void);
public:
    cNopacityRecordingMenuItem(cOsd *osd, const cRecording *Recording, bool sel, bool isFolder, int Level, int Total, int New);
    ~cNopacityRecordingMenuItem(void);
    void CreatePixmapTextScroller(int totalWidth);
    void CreateText(void);
    int CheckScrollable(bool hasIcon);
    void Render();
};

class cNopacityDefaultMenuItem : public cNopacityMenuItem {
private:
    std::string strEntry;
    std::string strEntryFull;
    int scrollCol;
    void SetTextFull(void);
    void SetTextShort(void);
public:
    cNopacityDefaultMenuItem(cOsd *osd, const char *text, bool sel);
    ~cNopacityDefaultMenuItem(void);
    int CheckScrollable(bool hasIcon);
    void Render();
};

class cNopacityTrackMenuItem : public cNopacityMenuItem {
private:
public:
    cNopacityTrackMenuItem(cOsd *osd, const char *text);
    ~cNopacityTrackMenuItem(void);
    void Render();
};
#endif //__NOPACITY_MENUITEM_H
