#include "textwindow.h"

cNopacityTextWindow::cNopacityTextWindow(cOsd *osd, cFont *font) {
    this->osd = osd;
    this->font = font;
    pixmap = NULL;
}

cNopacityTextWindow::~cNopacityTextWindow(void) {
    Cancel(-1);
    while (Active())
        cCondWait::SleepMs(10);
    if (pixmap) {
        osd->DestroyPixmap(pixmap);
        pixmap = NULL;
    }
}

bool cNopacityTextWindow::CreatePixmap(int border) {
    int lineHeight = font->Height();
    bool scrolling = false;
    twText.Set(text, font, geometry->Width() - 2*border);
    int pixmapTotalHeight = lineHeight * (twText.Lines()+1);
    int drawportHeight = geometry->Height();
    if (pixmapTotalHeight > drawportHeight) {
        drawportHeight = pixmapTotalHeight;
        scrolling = true;
    }
    cPixmap::Lock();
    pixmap = osd->CreatePixmap(4, cRect(geometry->X(), geometry->Y(), geometry->Width(), geometry->Height()),
                                  cRect(0, 0, geometry->Width(), drawportHeight));
    pixmap->SetAlpha(0);
    pixmap->Fill(Theme.Color(clrMenuBorder));
    pixmap->DrawRectangle(cRect(1, 1, geometry->Width()-2, drawportHeight-2), Theme.Color(clrMenuBack));
    cPixmap::Unlock();
    return scrolling;
}

void cNopacityTextWindow::DrawText(int border) {
    int lineHeight = font->Height();
    int currentLineHeight = lineHeight/2;
    cPixmap::Lock();
    for (int i=0; (i < twText.Lines()) && Running(); i++) {
        pixmap->DrawText(cPoint(border, currentLineHeight), twText.GetLine(i), Theme.Color(clrMenuFontButton), clrTransparent, font);
        currentLineHeight += lineHeight;
    }
    cPixmap::Unlock();        
}

void cNopacityTextWindow::DoSleep(int duration) {
    int sleepSlice = 10;
    for (int i = 0; Running() && (i*sleepSlice < duration); i++)
        cCondWait::SleepMs(sleepSlice);
}
   
void cNopacityTextWindow::Action(void) {
    DoSleep(config.menuInfoTextDelay*1000);
   
    int border = 5;
    bool scrolling = false;
    if (Running()) {
        scrolling = CreatePixmap(border);
    }
    if (Running()) {
        DrawText(border);
    }
    //FadeIn
    if (config.menuEPGWindowFadeTime) {
        uint64_t Start = cTimeMs::Now();
        int FadeTime = config.menuEPGWindowFadeTime;
        int FadeFrameTime = config.menuEPGWindowFrameTime;
        while (Running()) {
            uint64_t Now = cTimeMs::Now();
            cPixmap::Lock();
            double t = min(double(Now - Start) / FadeTime, 1.0);
            int Alpha = t * ALPHA_OPAQUE;
            pixmap->SetAlpha(Alpha);
            if (Running())
                osd->Flush();
            cPixmap::Unlock();
            int Delta = cTimeMs::Now() - Now;
            if (Running() && (Delta < FadeFrameTime))
                cCondWait::SleepMs(FadeFrameTime - Delta);
            if ((int)(Now - Start) > FadeTime)
                break;
        }
    }
    
    if (scrolling && Running()) {
        int scrollDelay = config.menuInfoScrollDelay * 1000;
        DoSleep(scrollDelay);
        int drawPortY;
        int FrameTime = config.menuInfoScrollFrameTime;
        int maxY = pixmap->DrawPort().Height() - pixmap->ViewPort().Height();
        bool doSleep = false;
        while (Running()) {
            uint64_t Now = cTimeMs::Now();
            cPixmap::Lock();
            drawPortY = pixmap->DrawPort().Y();
            drawPortY -= 1;
            cPixmap::Unlock();
            if (abs(drawPortY) > maxY) {
                doSleep = true;
                DoSleep(scrollDelay);
                drawPortY = 0;
            }
            cPixmap::Lock();
            if (Running())
                pixmap->SetDrawPortPoint(cPoint(0, drawPortY));
            cPixmap::Unlock();
            if (doSleep) {
                doSleep = false;
                DoSleep(scrollDelay);
            }
            int Delta = cTimeMs::Now() - Now;
            if (Running())
                osd->Flush();
            if (Running() && (Delta < FrameTime))
                cCondWait::SleepMs(FrameTime - Delta);
        }
    }
}