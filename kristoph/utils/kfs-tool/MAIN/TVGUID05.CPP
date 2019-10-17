#include <stdlib.h>               // for random()
#include <string.h>

#define Uses_TEventQueue
#define Uses_TEvent
#define Uses_TApplication
#define Uses_TKeys
#define Uses_TRect
#define Uses_TMenuBar
#define Uses_TSubMenu
#define Uses_TMenuItem
#define Uses_TStatusLine
#define Uses_TStatusItem
#define Uses_TStatusDef
#define Uses_TDeskTop
#define Uses_TView
#define Uses_TWindow
#include <tv.h>

const int cmMyNewWin   = 200;
const int cmMyKFSShowInfo = 201;
const int cmMyKFSFormat = 202;
const int cmMyKFSShowFiles = 203;

class TMyApp : public TApplication
{

public:
    TMyApp();
    static TStatusLine *initStatusLine( TRect r );
    static TMenuBar *initMenuBar( TRect r );
    virtual void handleEvent( TEvent& event);
    void myNewWindow();
};


static short winNumber = 0;           // initialize window number

class TDemoWindow : public TWindow    // define a new window class
{
public:
    TDemoWindow( const TRect& r, const char *aTitle, short aNumber );
};

class TInterior : public TView
{
public:
    TInterior( const TRect& bounds ); // constructor
    virtual void draw();              // override TView::draw
};

TInterior::TInterior( const TRect& bounds ) : TView( bounds )
{
    growMode = gfGrowHiX | gfGrowHiY; //make size follow that of the window
    options = options | ofFramed;
}

void TInterior::draw()
{
    TDrawBuffer b; // premenna b typu TDrawBuffer
    char *hstr = "KFS simulator v0.01a";
    char *pstr = "(c) Copyright 2003, Peter Jakubco ml.";
    ushort color = getColor(0x0301);
    TView::draw();
    b.moveStr(0, hstr, color);
    writeLine(2, 2, strlen(hstr), 1, b); // writeline: x,y,width,height,?)
    b.moveStr(0, pstr, color);
    writeLine(2, 4, strlen(pstr), 1, b); // writeline: x,y,width,height,?)
}

TMyApp::TMyApp() :
    TProgInit( &TMyApp::initStatusLine,
	       &TMyApp::initMenuBar,
	       &TMyApp::initDeskTop
	     )
{
}

TStatusLine *TMyApp::initStatusLine(TRect r)
{
    r.a.y = r.b.y - 1;     // move top to 1 line above bottom
    return new TStatusLine( r,
	*new TStatusDef( 0, 0xFFFF ) +
	// set range of help contexts
	    *new TStatusItem( 0, kbF10, cmMenu ) +
	    // define an item
	    *new TStatusItem( "~Alt+X~ Koniec", kbAltX, cmQuit ) +
	    // define an item
	    *new TStatusItem( "~Alt+F3~ Zatvorit", kbAltF3, cmClose )
            // and another one
        );
}

TMenuBar *TMyApp::initMenuBar( TRect r )
{

    r.b.y = r.a.y + 1;    // set bottom line 1 line below top line
    return new TMenuBar( r,
	*new TSubMenu("~S~ubor", kbAltS )+
	    *new TMenuItem("O ~p~rograme...",  cmMyNewWin,   kbF1, hcNoContext, "F1" )+
	    newLine()+
	    *new TMenuItem("~K~oniec", cmQuit, cmQuit, hcNoContext, "Alt+X" )+
	*new TSubMenu("~K~FS", kbAltK)+
	    *new TMenuItem("~Z~obrazit info", cmMyKFSShowInfo,kbAltF5,hcNoContext, "Alt+F5")+
	    *new TMenuItem("~V~ytvorit novy KFS", cmMyKFSFormat,kbAltF2,hcNoContext, "Alt+F2")+
	    *new TMenuItem("~P~ozriet subory", cmMyKFSShowFiles,kbF3,hcNoContext, "F3")+
	*new TSubMenu( "~O~kno", kbAltO )+
	    *new TMenuItem( "~D~alsie", cmNext,kbF6, hcNoContext, "F6" )+
	    *new TMenuItem( "~Z~vacsit", cmZoom,kbF5, hcNoContext, "F5" )
	);
}

void TMyApp::handleEvent(TEvent& event)
{
    TApplication::handleEvent(event);   // act like base!
    if( event.what == evCommand )
        {
        switch( event.message.command )
            {
            case cmMyNewWin:            // but respond to additional commands
                myNewWindow();          // define action for cmMyNewWin
                                        // command
                break;
            default:
                return;
            }
        clearEvent( event );            // clear event after handling
        }
}

void TMyApp::myNewWindow()
{
    TRect r((80/2)-(50/2),(25/2)-(8/2),(80/2)+(50/2),(25/2)+(8/2)); // set initial size and position
//    r.move( random(53), random(16) );   // randomly move around screen
//    TDemoWindow *window = new TDemoWindow ( r, "O programe", ++winNumber);
    TDemoWindow *window = new TDemoWindow ( r, "O programe",0);
    deskTop->insert(window);    // put window into desktop and draw it
}


TDemoWindow::TDemoWindow( const TRect& bounds, const char *aTitle,
              short aNumber) :
         TWindow( bounds, aTitle, aNumber),
         TWindowInit( &TDemoWindow::initFrame )
{
    TRect r = getClipRect();    // get exposed area
    r.grow(-1, -1);             // make interior fit inside window frame
    insert( new TInterior(r) ); // add interior to window
}


int main()
{
    TMyApp myApp;
    myApp.run();
    return 0;
}
