/*  MiniDock 1.1 - tiny macOS-style dock for Windows (x64).  Made by Wangcy
 *  Pure Win32 + GDI+ flat API; portable; config stored next to the exe.
 */
#define UNICODE
#define _UNICODE
#define _WIN32_WINNT 0x0601
#define WINVER 0x0601
#define COBJMACROS
#include <windows.h>
#include <windowsx.h>
#include <shellapi.h>
#include <shlobj.h>
#include <shlwapi.h>
#include <commctrl.h>
#include <commoncontrols.h>
#include <commdlg.h>
#include <urlmon.h>
#include <oleidl.h>
#include <dwmapi.h>
#include <math.h>
#include <wchar.h>
#include <psapi.h>

#define APPVER L"1.2.0"

/* ---------------- minimal GDI+ flat API ---------------- */
typedef int GpStatus;
typedef float REAL;
typedef DWORD ARGB;
typedef void GpGraphics, GpImage, GpBitmap, GpBrush, GpPath, GpPen, GpFont, GpFontFamily, GpStringFormat, GpImageAttributes;
typedef struct { UINT32 GdiplusVersion; void *DebugEventCallback; BOOL SuppressBackgroundThread; BOOL SuppressExternalCodecs; } GdiplusStartupInput;
typedef struct { REAL X, Y, Width, Height; } RectF;
typedef struct { INT X, Y, Width, Height; } GpRect;
typedef struct { UINT Width, Height; INT Stride; INT PixelFormat; void *Scan0; UINT_PTR Reserved; } GpBitmapData;
#define GPAPI WINAPI
#define PXF_PARGB 0xE200B
#define PXF_ARGB 0x26200A
GpStatus GPAPI GdiplusStartup(ULONG_PTR *, const GdiplusStartupInput *, void *);
void GPAPI GdiplusShutdown(ULONG_PTR);
GpStatus GPAPI GdipCreateBitmapFromScan0(INT, INT, INT, INT, BYTE *, GpBitmap **);
GpStatus GPAPI GdipCreateBitmapFromHICON(HICON, GpBitmap **);
GpStatus GPAPI GdipCreateBitmapFromFile(const WCHAR *, GpBitmap **);
GpStatus GPAPI GdipBitmapLockBits(GpBitmap *, const GpRect *, UINT, INT, GpBitmapData *);
GpStatus GPAPI GdipBitmapUnlockBits(GpBitmap *, GpBitmapData *);
GpStatus GPAPI GdipGetImageWidth(GpImage *, UINT *);
GpStatus GPAPI GdipGetImageHeight(GpImage *, UINT *);
GpStatus GPAPI GdipDisposeImage(GpImage *);
GpStatus GPAPI GdipGetImageGraphicsContext(GpImage *, GpGraphics **);
GpStatus GPAPI GdipCreateFromHDC(HDC, GpGraphics **);
GpStatus GPAPI GdipDeleteGraphics(GpGraphics *);
GpStatus GPAPI GdipGraphicsClear(GpGraphics *, ARGB);
GpStatus GPAPI GdipSetSmoothingMode(GpGraphics *, INT);
GpStatus GPAPI GdipSetInterpolationMode(GpGraphics *, INT);
GpStatus GPAPI GdipSetPixelOffsetMode(GpGraphics *, INT);
GpStatus GPAPI GdipSetTextRenderingHint(GpGraphics *, INT);
GpStatus GPAPI GdipResetWorldTransform(GpGraphics *);
GpStatus GPAPI GdipTranslateWorldTransform(GpGraphics *, REAL, REAL, INT);
GpStatus GPAPI GdipSetClipPath(GpGraphics *, GpPath *, INT);
GpStatus GPAPI GdipResetClip(GpGraphics *);
GpStatus GPAPI GdipCreateSolidFill(ARGB, GpBrush **);
GpStatus GPAPI GdipDeleteBrush(GpBrush *);
GpStatus GPAPI GdipCreateLineBrushFromRect(const RectF *, ARGB, ARGB, INT, INT, GpBrush **);
GpStatus GPAPI GdipCreatePen1(ARGB, REAL, INT, GpPen **);
GpStatus GPAPI GdipDeletePen(GpPen *);
GpStatus GPAPI GdipCreatePath(INT, GpPath **);
GpStatus GPAPI GdipDeletePath(GpPath *);
GpStatus GPAPI GdipAddPathArc(GpPath *, REAL, REAL, REAL, REAL, REAL, REAL);
GpStatus GPAPI GdipClosePathFigure(GpPath *);
GpStatus GPAPI GdipFillPath(GpGraphics *, GpBrush *, GpPath *);
GpStatus GPAPI GdipDrawPath(GpGraphics *, GpPen *, GpPath *);
GpStatus GPAPI GdipFillRectangle(GpGraphics *, GpBrush *, REAL, REAL, REAL, REAL);
GpStatus GPAPI GdipFillEllipse(GpGraphics *, GpBrush *, REAL, REAL, REAL, REAL);
GpStatus GPAPI GdipDrawLine(GpGraphics *, GpPen *, REAL, REAL, REAL, REAL);
GpStatus GPAPI GdipDrawImageRectRect(GpGraphics *, GpImage *, REAL, REAL, REAL, REAL, REAL, REAL, REAL, REAL, INT, const GpImageAttributes *, void *, void *);
GpStatus GPAPI GdipCreateImageAttributes(GpImageAttributes **);
GpStatus GPAPI GdipSetImageAttributesWrapMode(GpImageAttributes *, INT, ARGB, BOOL);
GpStatus GPAPI GdipSetImageAttributesColorMatrix(GpImageAttributes *, INT, BOOL, const float *, const float *, INT);
GpStatus GPAPI GdipCreateFontFamilyFromName(const WCHAR *, void *, GpFontFamily **);
GpStatus GPAPI GdipGetGenericFontFamilySansSerif(GpFontFamily **);
GpStatus GPAPI GdipCreateFont(const GpFontFamily *, REAL, INT, INT, GpFont **);
GpStatus GPAPI GdipDeleteFont(GpFont *);
GpStatus GPAPI GdipCreateStringFormat(INT, LANGID, GpStringFormat **);
GpStatus GPAPI GdipSetStringFormatAlign(GpStringFormat *, INT);
GpStatus GPAPI GdipSetStringFormatLineAlign(GpStringFormat *, INT);
GpStatus GPAPI GdipSetStringFormatTrimming(GpStringFormat *, INT);
GpStatus GPAPI GdipDrawString(GpGraphics *, const WCHAR *, INT, const GpFont *, const RectF *, const GpStringFormat *, const GpBrush *);
GpStatus GPAPI GdipDrawImagePointsRect(GpGraphics *, GpImage *, const float *, INT, REAL, REAL, REAL, REAL, INT, const GpImageAttributes *, void *, void *);
GpStatus GPAPI GdipFillPie(GpGraphics *, GpBrush *, REAL, REAL, REAL, REAL, REAL, REAL);
GpStatus GPAPI GdipMeasureString(GpGraphics *, const WCHAR *, INT, const GpFont *, const RectF *, const GpStringFormat *, RectF *, INT *, INT *);

static const GUID MY_IID_IImageList = {0x46EB5926,0x582E,0x4017,{0x9F,0xDF,0xE8,0x99,0x8D,0xAA,0x09,0x50}};
#define ARGBC(a, r, g, b) ((ARGB)(((DWORD)(a) << 24) | ((DWORD)(r) << 16) | ((DWORD)(g) << 8) | (DWORD)(b)))
#define PI_F 3.14159265f

/* ---------------- types & globals ---------------- */
#define MAXI 128
#define MAXK 16
#define WM_APP_ICON (WM_APP + 1)
#define WM_APP_TRAY (WM_APP + 2)
#define WM_APP_AB   (WM_APP + 3)
#define WM_APP_FAIL (WM_APP + 4)
#define T_ANIM 1
#define T_SCAN 2
#define T_HIDE 3
#define T_SAVE 4
#define T_TRIM 5
#define T_HOVER 6
#define T_MIN 7
#define T_CLOCK 8
#define CLS_DOCK L"MiniDockWnd"
#define CLS_SET  L"MiniDockSettings"
#define CLS_IN   L"MiniDockInput"
#define CLS_MENU L"MiniDockMenu"
#define CLS_GRP  L"MiniDockGroup"
#define CLS_PV   L"MiniDockPreview"
#define CLS_FAN  L"MiniDockFan"
#define CLS_NOTE L"MiniDockNote"
#define CLS_ICOD L"MiniDockIconDlg"
#define CLS_ANIM L"MiniDockAnim"

enum { T_FILE = 0, T_FOLDER = 1, T_URL = 2, T_SEP = 3, T_GROUP = 4, T_TRASH = 5, T_NOTE = 6 };
enum { E_BOTTOM = 0, E_TOP = 1, E_LEFT = 2, E_RIGHT = 3 };

typedef struct Item {
    int type, appmode, fav, temp, nk, fit;
    WCHAR args[260];
    WCHAR name[128];
    WCHAR path[1024];
    WCHAR icon_src[512];
    WCHAR target[MAX_PATH];
    GpBitmap *icon;
    float cur, slide, prevL;
    BOOL running;
    DWORD bounce;
    struct Item *kids;
} Item;

typedef struct {
    int iconSize, zoomSize, spacing, margin, pad, radius, opacity, blur;
    int zoom, theme, dots, labels, bounce, webApp, border, bgMode;
    int mode, reserve, edge, monitor, preview, showRunning, lock, hotkey;
    int dotStyle, plateDark, plateLight, plateRadius, bgColor1, bgColor2, accentTone;
    int reflect, clock, folderFan, minAnim, notes, uiTheme;
} Cfg;

static Cfg cfg;
static Item items[MAXI];
static int nItems;
static HINSTANCE g_inst;
static HWND g_hwnd, g_hset;
static ULONG_PTR g_gdipTok;
static int g_dpi = 96;
static WCHAR g_ini[MAX_PATH], g_dir[MAX_PATH], g_iconDir[MAX_PATH], g_exe[MAX_PATH];
static UINT g_taskbarMsg;
static int g_cacheSize = 96;
static BOOL g_noDl, g_dark = TRUE;
static HFONT g_uiFont, g_titleFont;
static BOOL g_uiDark;
static int g_menuEdge = -1;
static GpImageAttributes *g_attrFrost, *g_attrRefl, *g_attrRefl2;
static int g_clockW;

/* canvas / dock surface */
static HDC g_memdc;
static HBITMAP g_dib, g_oldbmp;
static BYTE *g_bits;
static GpBitmap *g_surf;
static GpGraphics *g_g;
static int winX, winY, winW, winH;      /* actual window rect */
static int LW, LH;                      /* logical (horizontal-layout) size */
static MONITORINFO g_mi;
static GpImageAttributes *g_attr, *g_attrFade;
static GpFont *g_font, *g_fontBold;
static GpStringFormat *g_fmtC, *g_fmtL, *g_fmtR;
/* frosted background capture */
static HDC g_fdc;
static HBITMAP g_fbmp, g_fold;
static BYTE *g_fbits;
static GpBitmap *g_frost;
static int g_fw, g_fh;
static DWORD g_fhash;

/* layout state (logical coordinates) */
static float slotL[MAXI + 1], slotR[MAXI + 1], actC[MAXI + 1], baseC[MAXI + 1];
static int prevN = -1;
static float barL, barW, barTop, barBottom;
static float gHideCur;
static int gHideTgt;
static BOOL gForceHide;
static DWORD gLastNear;
static BOOL gMenuOpen, gAnimOn, gMoved, gFsHidden;
static float mouseU = -1, mouseV = -1;
static int hot = -1, mergeT = -1;
static BOOL hover;
static int pressIdx = -1;
static POINT pressPt;
static BOOL dragging, removing;
static int dragIdx = -1;
static POINT dragPtA;         /* actual client coords */
static int gapIdx = -1;
static float gapCur;
static HWND g_pv;
static int pvItem = -1;

typedef struct {
    float S, Z, g, padX, sepW, pad, barH, overhang, margin;
    int lw, lh;
} Metrics;
static Metrics M;

/* ---------------- prototypes ---------------- */
static void Kick(void);
static BOOL RenderFrame(void);
static void ScheduleSave(void);
static void UpdateGeometry(void);
static void ApplySettings(void);
static void ShowSettings(int page);
static void LaunchItem(int i, BOOL force);
static void SaveNow(void);
static void LoadItems(void);
static void LoadConfig(void);
static void ReloadAllIcons(void);
static void LoadItemIcon(Item *it);
static void ClosePreview(void);
static void ShowGroup(int idx);
static void ShowFan(int idx);
static void ShowNote(void);
static void SetMinHook(BOOL on);
static void RefreshAccent(void);
static BOOL BuildFromPidl(Item *it, LPCITEMIDLIST pidl);
static LRESULT CALLBACK DockProc(HWND, UINT, WPARAM, LPARAM);

/* ---------------- helpers ---------------- */
static int Sc(int v) { return MulDiv(v, g_dpi, 96); }
static float Clampf(float v, float a, float b) { return v < a ? a : (v > b ? b : v); }

static const struct { const WCHAR *k; int *p; } g_tab[] = {
    { L"IconSize", &cfg.iconSize }, { L"ZoomSize", &cfg.zoomSize }, { L"Spacing", &cfg.spacing }, { L"Margin", &cfg.margin },
    { L"Padding", &cfg.pad }, { L"Radius", &cfg.radius }, { L"Opacity", &cfg.opacity }, { L"Blur", &cfg.blur },
    { L"Zoom", &cfg.zoom }, { L"Theme", &cfg.theme }, { L"Dots", &cfg.dots }, { L"Labels", &cfg.labels },
    { L"Bounce", &cfg.bounce }, { L"WebAppMode", &cfg.webApp }, { L"Border", &cfg.border }, { L"BgMode", &cfg.bgMode },
    { L"Mode", &cfg.mode }, { L"Reserve", &cfg.reserve }, { L"Edge", &cfg.edge }, { L"Monitor", &cfg.monitor },
    { L"Preview", &cfg.preview }, { L"ShowRunning", &cfg.showRunning }, { L"Lock", &cfg.lock }, { L"Hotkey", &cfg.hotkey },
    { L"DotStyle", &cfg.dotStyle }, { L"PlateDark", &cfg.plateDark }, { L"PlateLight", &cfg.plateLight }, { L"PlateRadius", &cfg.plateRadius },
    { L"BgColor1", &cfg.bgColor1 }, { L"BgColor2", &cfg.bgColor2 }, { L"AccentTone", &cfg.accentTone }, { L"Reflect", &cfg.reflect },
    { L"Clock", &cfg.clock }, { L"FolderFan", &cfg.folderFan }, { L"MinAnim", &cfg.minAnim }, { L"Notes", &cfg.notes }, { L"UiTheme", &cfg.uiTheme } };
#define NTAB ((int)(sizeof g_tab / sizeof g_tab[0]))

static void DefaultCfg(void) {
    cfg.iconSize = Sc(48); cfg.zoomSize = Sc(84); cfg.spacing = Sc(6); cfg.margin = Sc(8); cfg.pad = Sc(8);
    cfg.radius = Sc(18); cfg.opacity = 60; cfg.blur = 6;
    cfg.zoom = 1; cfg.theme = 0; cfg.dots = 1; cfg.labels = 1; cfg.bounce = 1; cfg.webApp = 1; cfg.border = 1; cfg.bgMode = 2;
    cfg.mode = 0; cfg.reserve = 0; cfg.edge = 0; cfg.monitor = 0; cfg.preview = 1; cfg.showRunning = 1; cfg.lock = 0; cfg.hotkey = 1;
    cfg.dotStyle = 0; cfg.plateDark = 0; cfg.plateLight = 0; cfg.plateRadius = 22; cfg.bgColor1 = -1; cfg.bgColor2 = -1; cfg.accentTone = 0;
    cfg.reflect = 0; cfg.clock = 0; cfg.folderFan = 1; cfg.minAnim = 1; cfg.notes = 0; cfg.uiTheme = 2;
}

static void EnsureIni(void) {
    if (GetFileAttributesW(g_ini) != INVALID_FILE_ATTRIBUTES) return;
    HANDLE h = CreateFileW(g_ini, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) { WORD bom = 0xFEFF; DWORD w; WriteFile(h, &bom, 2, &w, NULL); CloseHandle(h); }
}

static void InitPaths(void) {
    GetModuleFileNameW(NULL, g_exe, MAX_PATH);
    WCHAR exeDir[MAX_PATH], ap[MAX_PATH], local[MAX_PATH];
    lstrcpyW(exeDir, g_exe);
    PathRemoveFileSpecW(exeDir);
    SHGetFolderPathW(NULL, CSIDL_APPDATA, NULL, 0, ap);
    PathCombineW(g_dir, ap, L"MiniDock");
    CreateDirectoryW(g_dir, NULL);
    PathCombineW(g_ini, g_dir, L"MiniDock.ini");
    PathCombineW(local, exeDir, L"MiniDock.ini");
    /* settings live in %APPDATA%\\MiniDock so they survive moving / re-downloading the exe;
       an existing ini next to the exe is migrated once, and a file named MiniDock.portable forces portable mode */
    WCHAR marker[MAX_PATH];
    PathCombineW(marker, exeDir, L"MiniDock.portable");
    if (GetFileAttributesW(marker) != INVALID_FILE_ATTRIBUTES) {
        lstrcpyW(g_dir, exeDir);
        lstrcpyW(g_ini, local);
    } else if (GetFileAttributesW(g_ini) == INVALID_FILE_ATTRIBUTES && GetFileAttributesW(local) != INVALID_FILE_ATTRIBUTES) {
        CopyFileW(local, g_ini, TRUE);
        WCHAR li[MAX_PATH], di[MAX_PATH];
        PathCombineW(li, exeDir, L"icons");
        PathCombineW(di, g_dir, L"icons");
        CreateDirectoryW(di, NULL);
        WIN32_FIND_DATAW fd;
        WCHAR pat[MAX_PATH];
        PathCombineW(pat, li, L"*");
        HANDLE ff = FindFirstFileW(pat, &fd);
        if (ff != INVALID_HANDLE_VALUE) {
            do {
                if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue;
                WCHAR a[MAX_PATH], b[MAX_PATH];
                PathCombineW(a, li, fd.cFileName);
                PathCombineW(b, di, fd.cFileName);
                CopyFileW(a, b, TRUE);
            } while (FindNextFileW(ff, &fd));
            FindClose(ff);
        }
    }
    EnsureIni();
    PathCombineW(g_iconDir, g_dir, L"icons");
    CreateDirectoryW(g_iconDir, NULL);
}

static BOOL IsUrlStr(const WCHAR *s) {
    return StrCmpNIW(s, L"http://", 7) == 0 || StrCmpNIW(s, L"https://", 8) == 0 || StrCmpNIW(s, L"www.", 4) == 0;
}
static void HostOf(const WCHAR *url, WCHAR *out, int cch) {
    const WCHAR *p = wcsstr(url, L"://");
    p = p ? p + 3 : url;
    int n = 0;
    while (*p && *p != L'/' && *p != L':' && *p != L'?' && *p != L'#' && n < cch - 1) out[n++] = *p++;
    out[n] = 0;
}
static DWORD Fnv(const WCHAR *s) {
    DWORD h = 2166136261u;
    for (; *s; s++) { h ^= (WORD)*s; h *= 16777619u; }
    return h;
}
static void IconFileFor(const WCHAR *url, WCHAR *out) {
    WCHAR host[256], nm[40];
    HostOf(url, host, 256);
    CharLowerW(host);
    wsprintfW(nm, L"%08x.png", Fnv(host));
    PathCombineW(out, g_iconDir, nm);
}

/* ---------------- geometry mapping (edge aware) ---------------- */
static void MapR(float u, float v, float w, float h, float *x, float *y, float *ww, float *hh) {
    switch (cfg.edge) {
    case E_TOP:   *x = u; *y = LH - v - h; *ww = w; *hh = h; break;
    case E_LEFT:  *x = LH - v - h; *y = u; *ww = h; *hh = w; break;
    case E_RIGHT: *x = v; *y = u; *ww = h; *hh = w; break;
    default:      *x = u; *y = v; *ww = w; *hh = h; break;
    }
}
static void ToLogical(float px, float py, float *u, float *v) {
    switch (cfg.edge) {
    case E_TOP:   *u = px; *v = LH - py; break;
    case E_LEFT:  *u = py; *v = LH - px; break;
    case E_RIGHT: *u = py; *v = px; break;
    default:      *u = px; *v = py; break;
    }
}
static BOOL Horiz(void) { return cfg.edge == E_BOTTOM || cfg.edge == E_TOP; }

/* ---------------- theme ---------------- */
static BOOL SystemDark(void) {
    DWORD v = 1, sz = 4;
    if (RegGetValueW(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize", L"AppsUseLightTheme", RRF_RT_REG_DWORD, NULL, &v, &sz) != ERROR_SUCCESS) v = 1;
    return v == 0;
}
static void UpdateTheme(void) {
    g_dark = cfg.theme == 0 ? TRUE : cfg.theme == 1 ? FALSE : SystemDark();
    g_uiDark = cfg.uiTheme == 0 ? FALSE : cfg.uiTheme == 1 ? TRUE : SystemDark();
}

/* ---------------- config ---------------- */
static void LoadConfig(void) {
    DefaultCfg();
    if ((int)GetPrivateProfileIntW(L"Settings", L"BgMode", -1, g_ini) < 0) cfg.bgMode = GetPrivateProfileIntW(L"Settings", L"Frost", 1, g_ini) ? 2 : 0;
    for (int i = 0; i < NTAB; i++) *g_tab[i].p = (int)GetPrivateProfileIntW(L"Settings", g_tab[i].k, *g_tab[i].p, g_ini);
    if (cfg.bgMode < 0 || cfg.bgMode > 2) cfg.bgMode = 2;
    if (cfg.opacity < 0) cfg.opacity = 0;
    if (cfg.iconSize < 16) cfg.iconSize = 16;
    if (cfg.iconSize > 256) cfg.iconSize = 256;
    if (cfg.edge < 0 || cfg.edge > 3) cfg.edge = 0;
    if (cfg.mode < 0 || cfg.mode > 2) cfg.mode = 0;
    UpdateTheme();
}

static void SaveSettingsOnly(void) {
    for (int i = 0; i < NTAB; i++) {
        WCHAR b[24];
        wsprintfW(b, L"%d", *g_tab[i].p);
        WritePrivateProfileStringW(L"Settings", g_tab[i].k, b, g_ini);
    }
}

static void ScheduleSave(void) { if (g_hwnd) SetTimer(g_hwnd, T_SAVE, 700, NULL); }

#define RUNKEY L"Software\\Microsoft\\Windows\\CurrentVersion\\Run"
static BOOL GetAutostart(void) {
    HKEY k; BOOL r = FALSE;
    if (RegOpenKeyExW(HKEY_CURRENT_USER, RUNKEY, 0, KEY_READ, &k) == ERROR_SUCCESS) {
        r = RegQueryValueExW(k, L"MiniDock", NULL, NULL, NULL, NULL) == ERROR_SUCCESS;
        RegCloseKey(k);
    }
    return r;
}
static void SetAutostart(BOOL on) {
    HKEY k;
    if (RegCreateKeyExW(HKEY_CURRENT_USER, RUNKEY, 0, NULL, 0, KEY_WRITE, NULL, &k, NULL) != ERROR_SUCCESS) return;
    if (on) {
        WCHAR v[MAX_PATH + 4];
        wsprintfW(v, L"\"%s\"", g_exe);
        RegSetValueExW(k, L"MiniDock", 0, REG_SZ, (BYTE *)v, (lstrlenW(v) + 1) * 2);
    } else RegDeleteValueW(k, L"MiniDock");
    RegCloseKey(k);
}

/* ---------------- canvas for popup windows (menu, group) ---------------- */
typedef struct { HWND h; HDC dc; HBITMAP bm, old; BYTE *bits; GpBitmap *surf; GpGraphics *g; int w, h2; } Canvas;

static void SetupG(GpGraphics *g) {
    GdipSetSmoothingMode(g, 4);
    GdipSetInterpolationMode(g, 7);
    GdipSetPixelOffsetMode(g, 4);
    GdipSetTextRenderingHint(g, 4);
}
static BOOL CanvasNew(Canvas *c, int w, int h) {
    ZeroMemory(c, sizeof *c);
    c->w = w; c->h2 = h;
    HDC s = GetDC(NULL);
    c->dc = CreateCompatibleDC(s);
    ReleaseDC(NULL, s);
    BITMAPINFO bi;
    ZeroMemory(&bi, sizeof bi);
    bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bi.bmiHeader.biWidth = w; bi.bmiHeader.biHeight = -h; bi.bmiHeader.biPlanes = 1; bi.bmiHeader.biBitCount = 32;
    c->bm = CreateDIBSection(c->dc, &bi, DIB_RGB_COLORS, (void **)&c->bits, NULL, 0);
    if (!c->bm) return FALSE;
    c->old = (HBITMAP)SelectObject(c->dc, c->bm);
    GdipCreateBitmapFromScan0(w, h, w * 4, PXF_PARGB, c->bits, &c->surf);
    GdipGetImageGraphicsContext(c->surf, &c->g);
    SetupG(c->g);
    return TRUE;
}
static void CanvasFree(Canvas *c) {
    if (c->g) GdipDeleteGraphics(c->g);
    if (c->surf) GdipDisposeImage(c->surf);
    if (c->bm) { SelectObject(c->dc, c->old); DeleteObject(c->bm); }
    if (c->dc) DeleteDC(c->dc);
    ZeroMemory(c, sizeof *c);
}
static void CanvasPresent(Canvas *c, HWND h, int x, int y) {
    POINT pt = { x, y }, src = { 0, 0 };
    SIZE sz = { c->w, c->h2 };
    BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
    UpdateLayeredWindow(h, NULL, &pt, &sz, c->dc, &src, 0, &bf, ULW_ALPHA);
}

/* ---------------- drawing primitives ---------------- */
static void DrawRoundRectPath(GpPath *p, float x, float y, float w, float h, float r) {
    float d = r * 2;
    if (d > w) d = w;
    if (d > h) d = h;
    if (d < 0.5f) d = 0.5f;
    GdipAddPathArc(p, x, y, d, d, 180, 90);
    GdipAddPathArc(p, x + w - d, y, d, d, 270, 90);
    GdipAddPathArc(p, x + w - d, y + h - d, d, d, 0, 90);
    GdipAddPathArc(p, x, y + h - d, d, d, 90, 90);
    GdipClosePathFigure(p);
}
static void FillRoundRect(GpGraphics *g, GpBrush *b, float x, float y, float w, float h, float r) {
    GpPath *p;
    GdipCreatePath(0, &p);
    DrawRoundRectPath(p, x, y, w, h, r);
    GdipFillPath(g, b, p);
    GdipDeletePath(p);
}
static void FillRR(GpGraphics *g, ARGB c, float x, float y, float w, float h, float r) {
    GpBrush *b;
    GdipCreateSolidFill(c, &b);
    FillRoundRect(g, b, x, y, w, h, r);
    GdipDeleteBrush(b);
}
static void StrokeRR(GpGraphics *g, ARGB c, float wd, float x, float y, float w, float h, float r) {
    GpPen *pen;
    GpPath *p;
    GdipCreatePen1(c, wd, 2, &pen);
    GdipCreatePath(0, &p);
    DrawRoundRectPath(p, x + wd / 2, y + wd / 2, w - wd, h - wd, r);
    GdipDrawPath(g, pen, p);
    GdipDeletePath(p);
    GdipDeletePen(pen);
}
static void DrawImg(GpGraphics *g, GpBitmap *src, float x, float y, float w, float h, BOOL fade) {
    UINT sw = 0, sh = 0;
    GdipGetImageWidth(src, &sw);
    GdipGetImageHeight(src, &sh);
    GdipDrawImageRectRect(g, src, x, y, w, h, 0, 0, (REAL)sw, (REAL)sh, 2, fade ? g_attrFade : g_attr, NULL, NULL);
}
static void DrawTextBox(GpGraphics *g, const WCHAR *t, GpFont *f, float x, float y, float w, float h, ARGB col, BOOL left) {
    GpBrush *br;
    GdipCreateSolidFill(col, &br);
    RectF rc = { x, y, w, h };
    GdipDrawString(g, t, -1, f, &rc, left ? g_fmtL : g_fmtC, br);
    GdipDeleteBrush(br);
}
static void DrawTextBoxR(GpGraphics *g, const WCHAR *t, GpFont *f, float x, float y, float w, float h, ARGB col) {
    GpBrush *br;
    GdipCreateSolidFill(col, &br);
    RectF rc = { x, y, w, h };
    GdipDrawString(g, t, -1, f, &rc, g_fmtR, br);
    GdipDeleteBrush(br);
}
static void DrawCentered(GpGraphics *g, const WCHAR *t, GpFont *f, float x, float y, float w, float h, ARGB col) {
    DrawTextBox(g, t, f, x, y, w, h, col, FALSE);
}
static float TextWidth(GpGraphics *g, const WCHAR *t, GpFont *f) {
    RectF lay = { 0, 0, 600, 40 }, bb = { 0, 0, 0, 0 };
    INT cp = 0, ln = 0;
    GdipMeasureString(g, t, -1, f, &lay, g_fmtL, &bb, &cp, &ln);
    return bb.Width;
}

/* ======================= icons ======================= */
static GpBitmap *NewCache(int s, GpGraphics **gout) {
    GpBitmap *b = NULL;
    GdipCreateBitmapFromScan0(s, s, 0, PXF_PARGB, NULL, &b);
    if (b) { GdipGetImageGraphicsContext(b, gout); SetupG(*gout); }
    return b;
}

static void AlphaBBox(GpBitmap *b, UINT w, UINT h, int *x0, int *y0, int *x1, int *y1, int thr) {
    *x0 = 0; *y0 = 0; *x1 = (int)w; *y1 = (int)h;
    GpRect r = { 0, 0, (INT)w, (INT)h };
    GpBitmapData d;
    if (GdipBitmapLockBits(b, &r, 1, PXF_ARGB, &d) != 0) return;
    int minx = (int)w, miny = (int)h, maxx = -1, maxy = -1;
    for (int y = 0; y < (int)h; y++) {
        BYTE *row = (BYTE *)d.Scan0 + (INT_PTR)y * d.Stride;
        for (int x = 0; x < (int)w; x++)
            if (row[x * 4 + 3] > thr) {
                if (x < minx) minx = x;
                if (x > maxx) maxx = x;
                if (y < miny) miny = y;
                if (y > maxy) maxy = y;
            }
    }
    GdipBitmapUnlockBits(b, &d);
    if (maxx >= 0) { *x0 = minx; *y0 = miny; *x1 = maxx + 1; *y1 = maxy + 1; }
}

/* find the bounding box of a uniformly coloured (e.g. black / white) border so it can be trimmed */
static void SolidBBox(GpBitmap *b, UINT w, UINT h, int *x0, int *y0, int *x1, int *y1) {
    GpRect r = { 0, 0, (INT)w, (INT)h };
    GpBitmapData d;
    if (GdipBitmapLockBits(b, &r, 1, PXF_ARGB, &d) != 0) return;
    BYTE *p0 = (BYTE *)d.Scan0;
    BYTE ref[4] = { p0[0], p0[1], p0[2], p0[3] };
    if (ref[3] < 200) { GdipBitmapUnlockBits(b, &d); return; }
    #define ISBG(px) ((px)[3] > 200 && abs((int)(px)[0] - ref[0]) + abs((int)(px)[1] - ref[1]) + abs((int)(px)[2] - ref[2]) < 70)
    int top = 0, bot = (int)h - 1, lef = 0, rig = (int)w - 1;
    for (; top < bot; top++) { BYTE *row = p0 + (INT_PTR)top * d.Stride; int bad = 0; for (int x = 0; x < (int)w; x++) if (!ISBG(row + x * 4)) bad++; if (bad * 100 > (int)w * 2) break; }
    for (; bot > top; bot--) { BYTE *row = p0 + (INT_PTR)bot * d.Stride; int bad = 0; for (int x = 0; x < (int)w; x++) if (!ISBG(row + x * 4)) bad++; if (bad * 100 > (int)w * 2) break; }
    for (; lef < rig; lef++) { int bad = 0; for (int y = top; y <= bot; y++) if (!ISBG(p0 + (INT_PTR)y * d.Stride + lef * 4)) bad++; if (bad * 100 > (bot - top + 1) * 2) break; }
    for (; rig > lef; rig--) { int bad = 0; for (int y = top; y <= bot; y++) if (!ISBG(p0 + (INT_PTR)y * d.Stride + rig * 4)) bad++; if (bad * 100 > (bot - top + 1) * 2) break; }
    GdipBitmapUnlockBits(b, &d);
    if (bot - top > 8 && rig - lef > 8) { *x0 = lef; *y0 = top; *x1 = rig + 1; *y1 = bot + 1; }
}

/* fit: 0 = auto (trim transparent / solid borders), 1 = keep whole image, 2 = centre-crop to fill */
static GpBitmap *FitBitmap(GpBitmap *src, int cs, int fit, float fill) {
    UINT w = 0, h = 0;
    GdipGetImageWidth(src, &w);
    GdipGetImageHeight(src, &h);
    if (!w || !h) return NULL;
    int x0 = 0, y0 = 0, x1 = (int)w, y1 = (int)h;
    BOOL full = FALSE;
    if (fit == 0) {
        AlphaBBox(src, w, h, &x0, &y0, &x1, &y1, 40);
        if (x0 == 0 && y0 == 0 && x1 == (int)w && y1 == (int)h) { SolidBBox(src, w, h, &x0, &y0, &x1, &y1); full = TRUE; }
    } else if (fit == 2) {
        int sq = (int)(w < h ? w : h);
        x0 = ((int)w - sq) / 2; y0 = ((int)h - sq) / 2; x1 = x0 + sq; y1 = y0 + sq;
        fill = 1.0f; full = TRUE;
    }
    float sw = (float)(x1 - x0), sh = (float)(y1 - y0);
    float mx = sw > sh ? sw : sh;
    float sc = cs * fill / mx, dw = sw * sc, dh = sh * sc;
    GpGraphics *g = NULL;
    GpBitmap *dst = NewCache(cs, &g);
    if (!dst) return NULL;
    if (full) { /* opaque square image -> round the corners like a real app icon */
        GpPath *cp;
        GdipCreatePath(0, &cp);
        DrawRoundRectPath(cp, (cs - dw) / 2, (cs - dh) / 2, dw, dh, (dw < dh ? dw : dh) * 0.22f);
        GdipSetClipPath(g, cp, 0);
        GdipDeletePath(cp);
    }
    GdipDrawImageRectRect(g, src, (cs - dw) / 2, (cs - dh) / 2, dw, dh, (REAL)x0, (REAL)y0, sw, sh, 2, g_attr, NULL, NULL);
    GdipDeleteGraphics(g);
    return dst;
}

/* HICON -> straight-alpha GDI+ bitmap (GdipCreateBitmapFromHICON leaves a black plate behind many icons) */
static GpBitmap *HiconToGp(HICON hi) {
    ICONINFO ii;
    GpBitmap *out = NULL;
    if (!GetIconInfo(hi, &ii)) return NULL;
    if (ii.hbmColor) {
        BITMAP bm;
        GetObjectW(ii.hbmColor, sizeof bm, &bm);
        int w = bm.bmWidth, h = bm.bmHeight;
        BITMAPINFO bi;
        ZeroMemory(&bi, sizeof bi);
        bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bi.bmiHeader.biWidth = w; bi.bmiHeader.biHeight = -h; bi.bmiHeader.biPlanes = 1; bi.bmiHeader.biBitCount = 32;
        BYTE *px = (BYTE *)HeapAlloc(GetProcessHeap(), 0, (SIZE_T)w * h * 4);
        if (px) {
            HDC dc = GetDC(NULL);
            GetDIBits(dc, ii.hbmColor, 0, h, px, &bi, DIB_RGB_COLORS);
            BOOL anyA = FALSE;
            for (int i = 0; i < w * h; i++) if (px[i * 4 + 3]) { anyA = TRUE; break; }
            if (!anyA && ii.hbmMask) {
                BYTE *mk = (BYTE *)HeapAlloc(GetProcessHeap(), 0, (SIZE_T)w * h * 4);
                if (mk) {
                    GetDIBits(dc, ii.hbmMask, 0, h, mk, &bi, DIB_RGB_COLORS);
                    for (int i = 0; i < w * h; i++) px[i * 4 + 3] = (mk[i * 4] == 0) ? 255 : 0;
                    HeapFree(GetProcessHeap(), 0, mk);
                }
            }
            ReleaseDC(NULL, dc);
            if (GdipCreateBitmapFromScan0(w, h, 0, PXF_ARGB, NULL, &out) == 0 && out) {
                GpRect r = { 0, 0, w, h };
                GpBitmapData d;
                if (GdipBitmapLockBits(out, &r, 2, PXF_ARGB, &d) == 0) {
                    for (int y = 0; y < h; y++) memcpy((BYTE *)d.Scan0 + (INT_PTR)y * d.Stride, px + (SIZE_T)y * w * 4, (SIZE_T)w * 4);
                    GdipBitmapUnlockBits(out, &d);
                } else { GdipDisposeImage(out); out = NULL; }
            }
            HeapFree(GetProcessHeap(), 0, px);
        }
        DeleteObject(ii.hbmColor);
    }
    if (ii.hbmMask) DeleteObject(ii.hbmMask);
    if (!out) GdipCreateBitmapFromHICON(hi, &out);
    return out;
}

static void HsvRgb(float h, float s, float v, int *r, int *g, int *b) {
    float c = v * s, hp = h / 60.f, x = c * (1 - fabsf(fmodf(hp, 2) - 1)), r1 = 0, g1 = 0, b1 = 0;
    if (hp < 1) { r1 = c; g1 = x; } else if (hp < 2) { r1 = x; g1 = c; } else if (hp < 3) { g1 = c; b1 = x; }
    else if (hp < 4) { g1 = x; b1 = c; } else if (hp < 5) { r1 = x; b1 = c; } else { r1 = c; b1 = x; }
    float m = v - c;
    *r = (int)((r1 + m) * 255); *g = (int)((g1 + m) * 255); *b = (int)((b1 + m) * 255);
}

static GpBitmap *LetterTile(const WCHAR *name, int cs) {
    GpGraphics *g = NULL;
    GpBitmap *b = NewCache(cs, &g);
    if (!b) return NULL;
    float hue = (float)(Fnv(name) % 360);
    int r1, g1, b1, r2, g2, b2;
    HsvRgb(hue, 0.55f, 0.95f, &r1, &g1, &b1);
    HsvRgb(hue, 0.75f, 0.70f, &r2, &g2, &b2);
    float m = cs * 0.06f;
    RectF rc = { m, m, cs - 2 * m, cs - 2 * m };
    GpBrush *br;
    GdipCreateLineBrushFromRect(&rc, ARGBC(255, r1, g1, b1), ARGBC(255, r2, g2, b2), 1, 0, &br);
    FillRoundRect(g, br, m, m, cs - 2 * m, cs - 2 * m, cs * 0.22f);
    GdipDeleteBrush(br);
    WCHAR ch[3] = { 0, 0, 0 };
    const WCHAR *p = name;
    if (StrCmpNIW(p, L"www.", 4) == 0) p += 4;
    ch[0] = *p ? *p : L'?';
    CharUpperBuffW(ch, 1);
    GpFontFamily *fam = NULL;
    GpFont *f = NULL;
    if (GdipCreateFontFamilyFromName(L"Microsoft YaHei UI", NULL, &fam) != 0) GdipGetGenericFontFamilySansSerif(&fam);
    GdipCreateFont(fam, cs * 0.5f, 1, 2, &f);
    DrawCentered(g, ch, f, 0, 0, (float)cs, (float)cs, ARGBC(255, 255, 255, 255));
    GdipDeleteFont(f);
    GdipDeleteGraphics(g);
    return b;
}

/* favicon / website icon file -> cache bitmap (small icons go on a white tile) */
static GpBitmap *FaviconBitmap(const WCHAR *file, int cs) {
    GpBitmap *src = NULL;
    if (GdipCreateBitmapFromFile(file, &src) != 0 || !src) return NULL;
    UINT w = 0, h = 0;
    GdipGetImageWidth(src, &w);
    GdipGetImageHeight(src, &h);
    if (w < 8 || h < 8) { GdipDisposeImage(src); return NULL; }
    GpBitmap *out = NULL;
    if (w < 80) {
        GpGraphics *g = NULL;
        out = NewCache(cs, &g);
        if (out) {
            float m = cs * 0.06f;
            RectF rc = { m, m, cs - 2 * m, cs - 2 * m };
            GpBrush *br;
            GdipCreateLineBrushFromRect(&rc, ARGBC(255, 255, 255, 255), ARGBC(255, 232, 234, 240), 1, 0, &br);
            FillRoundRect(g, br, m, m, cs - 2 * m, cs - 2 * m, cs * 0.22f);
            GdipDeleteBrush(br);
            float s = cs * 0.55f, o = (cs - s) / 2;
            DrawImg(g, src, o, o, s, s, FALSE);
            GdipDeleteGraphics(g);
        }
    } else out = FitBitmap(src, cs, 0, 0.94f);
    GdipDisposeImage(src);
    return out;
}

static HICON ShellHicon(const WCHAR *path, LPCITEMIDLIST pidl) {
    SHFILEINFOW sfi;
    ZeroMemory(&sfi, sizeof sfi);
    DWORD fl = SHGFI_SYSICONINDEX | (pidl ? SHGFI_PIDL : 0);
    if (!SHGetFileInfoW(pidl ? (LPCWSTR)pidl : path, 0, &sfi, sizeof sfi, fl)) return NULL;
    HICON hi = NULL;
    static const int lists[2] = { SHIL_JUMBO, SHIL_EXTRALARGE };
    for (int k = 0; k < 2 && !hi; k++) {
        IImageList *il = NULL;
        if (SUCCEEDED(SHGetImageList(lists[k], &MY_IID_IImageList, (void **)&il)) && il) {
            IImageList_GetIcon(il, sfi.iIcon, ILD_TRANSPARENT, &hi);
            IImageList_Release(il);
        }
    }
    return hi;
}

static GpBitmap *HiconToCache(HICON hi, int cs) {
    GpBitmap *src = HiconToGp(hi);
    DestroyIcon(hi);
    if (!src) return NULL;
    GpBitmap *b = FitBitmap(src, cs, 0, 0.94f);
    GdipDisposeImage(src);
    return b;
}

static GpBitmap *ShellIconBitmap(const WCHAR *path, LPCITEMIDLIST pidl, int cs) {
    HICON hi = ShellHicon(path, pidl);
    if (!hi && !pidl) {
        SHFILEINFOW sfi;
        ZeroMemory(&sfi, sizeof sfi);
        if (SHGetFileInfoW(path, 0, &sfi, sizeof sfi, SHGFI_ICON | SHGFI_LARGEICON)) hi = sfi.hIcon;
    }
    return hi ? HiconToCache(hi, cs) : NULL;
}

/* ---- shortcuts ---- */
static BOOL ReadLnk(const WCHAR *lnk, WCHAR *target, WCHAR *args, WCHAR *iconLoc, int *iconIdx) {
    target[0] = 0; args[0] = 0; iconLoc[0] = 0; *iconIdx = 0;
    IShellLinkW *psl = NULL;
    BOOL ok = FALSE;
    if (SUCCEEDED(CoCreateInstance(&CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, &IID_IShellLinkW, (void **)&psl))) {
        IPersistFile *ppf = NULL;
        if (SUCCEEDED(IShellLinkW_QueryInterface(psl, &IID_IPersistFile, (void **)&ppf))) {
            if (SUCCEEDED(IPersistFile_Load(ppf, lnk, STGM_READ))) {
                IShellLinkW_GetPath(psl, target, MAX_PATH, NULL, 0);
                IShellLinkW_GetArguments(psl, args, 1024);
                IShellLinkW_GetIconLocation(psl, iconLoc, MAX_PATH, iconIdx);
                ok = TRUE;
            }
            IPersistFile_Release(ppf);
        }
        IShellLinkW_Release(psl);
    }
    return ok;
}

static GpBitmap *LnkIconBitmap(const WCHAR *lnk, int cs) {
    WCHAR t[MAX_PATH], a[1024], il[MAX_PATH], ex[MAX_PATH];
    int idx;
    if (!ReadLnk(lnk, t, a, il, &idx) || !il[0]) return NULL;
    ExpandEnvironmentStringsW(il, ex, MAX_PATH);
    HICON hi = NULL;
    static const int sizes[3] = { 256, 128, 64 };
    for (int k = 0; k < 3 && !hi; k++) {
        UINT id = 0;
        if (PrivateExtractIconsW(ex, idx, sizes[k], sizes[k], &hi, &id, 1, 0) != 1) hi = NULL;
    }
    return hi ? HiconToCache(hi, cs) : NULL;
}

/* ---- downloads (favicons / custom icon links) ---- */
typedef struct { int mode; WCHAR host[256]; WCHAR url[1024]; WCHAR file[MAX_PATH]; } DlJob;

static UINT BmpWidth(const WCHAR *f) {
    GpBitmap *b = NULL;
    UINT w = 0;
    if (GdipCreateBitmapFromFile(f, &b) == 0 && b) { GdipGetImageWidth(b, &w); GdipDisposeImage(b); }
    return w;
}

static BOOL DlOne(const WCHAR *url, const WCHAR *file) {
    DeleteFileW(file);
    if (URLDownloadToFileW(NULL, url, file, 0, NULL) != S_OK) { DeleteFileW(file); return FALSE; }
    WIN32_FILE_ATTRIBUTE_DATA fa;
    if (!GetFileAttributesExW(file, GetFileExInfoStandard, &fa) || fa.nFileSizeLow < 60) { DeleteFileW(file); return FALSE; }
    return TRUE;
}

static DWORD WINAPI DlThread(LPVOID p) {
    DlJob *j = (DlJob *)p;
    CoInitializeEx(NULL, COINIT_MULTITHREADED);
    WCHAR tmp[MAX_PATH + 8];
    if (j->mode == 1) {
        lstrcpyW(tmp, j->file);
        lstrcatW(tmp, L".tmp");
        if (DlOne(j->url, tmp)) MoveFileExW(tmp, j->file, MOVEFILE_REPLACE_EXISTING);
    } else {
        static const WCHAR *fmts[4] = { L"https://www.google.com/s2/favicons?domain=%s&sz=128", L"https://%s/apple-touch-icon.png",
                                        L"https://%s/favicon.ico", L"https://icons.duckduckgo.com/ip3/%s.ico" };
        WCHAR cand[4][MAX_PATH + 8], u[700];
        UINT best = 0;
        int bi = -1;
        for (int k = 0; k < 4; k++) {
            wsprintfW(cand[k], L"%s.%d", j->file, k);
            wsprintfW(u, fmts[k], j->host);
            if (DlOne(u, cand[k])) {
                UINT w = BmpWidth(cand[k]);
                if (w > best) { best = w; bi = k; }
                if (w >= 120) break;
            }
        }
        if (bi >= 0) MoveFileExW(cand[bi], j->file, MOVEFILE_REPLACE_EXISTING);
        for (int k = 0; k < 4; k++) DeleteFileW(cand[k]);
    }
    if (g_hwnd) PostMessageW(g_hwnd, WM_APP_ICON, 0, 0);
    CoUninitialize();
    HeapFree(GetProcessHeap(), 0, j);
    return 0;
}

static void StartDownload(int mode, const WCHAR *url, const WCHAR *file) {
    DlJob *j = (DlJob *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(DlJob));
    if (!j) return;
    j->mode = mode;
    if (mode == 0) HostOf(url, j->host, 256); else lstrcpynW(j->url, url, 1024);
    lstrcpynW(j->file, file, MAX_PATH);
    HANDLE h = CreateThread(NULL, 0, DlThread, j, 0, NULL);
    if (h) CloseHandle(h); else HeapFree(GetProcessHeap(), 0, j);
}

/* ---- group thumbnail (first four apps in a 2x2 grid) ---- */
static void BuildGroupIcon(Item *g) {
    if (g->icon) { GdipDisposeImage(g->icon); g->icon = NULL; }
    int cs = g_cacheSize;
    GpGraphics *gr = NULL;
    GpBitmap *b = NewCache(cs, &gr);
    if (!b) return;
    float m = cs * 0.05f;
    RectF rc = { m, m, cs - 2 * m, cs - 2 * m };
    GpBrush *br;
    GdipCreateLineBrushFromRect(&rc, ARGBC(205, 238, 240, 248), ARGBC(205, 196, 201, 218), 1, 0, &br);
    FillRoundRect(gr, br, m, m, cs - 2 * m, cs - 2 * m, cs * 0.22f);
    GdipDeleteBrush(br);
    StrokeRR(gr, ARGBC(150, 255, 255, 255), 1.5f, m, m, cs - 2 * m, cs - 2 * m, cs * 0.22f);
    float cell = cs * 0.34f, gap = cs * 0.07f, o = (cs - 2 * cell - gap) / 2;
    for (int k = 0; k < g->nk && k < 4; k++)
        if (g->kids[k].icon) DrawImg(gr, g->kids[k].icon, o + (k % 2) * (cell + gap), o + (k / 2) * (cell + gap), cell, cell, FALSE);
    GdipDeleteGraphics(gr);
    g->icon = b;
}

static GpBitmap *NoteIcon(int cs) {
    GpGraphics *g = NULL;
    GpBitmap *b = NewCache(cs, &g);
    if (!b) return NULL;
    float m = cs * 0.06f, w = cs - 2 * m;
    RectF rc = { m, m, w, w };
    GpBrush *br;
    GdipCreateLineBrushFromRect(&rc, ARGBC(255, 255, 224, 110), ARGBC(255, 255, 190, 40), 1, 0, &br);
    FillRoundRect(g, br, m, m, w, w, cs * 0.22f);
    GdipDeleteBrush(br);
    FillRR(g, ARGBC(60, 120, 70, 0), m, m + w * 0.22f, w, 2, 0);
    GpPen *pn;
    GdipCreatePen1(ARGBC(150, 130, 80, 0), cs * 0.045f, 2, &pn);
    for (int i = 0; i < 4; i++) GdipDrawLine(g, pn, m + w * 0.2f, m + w * (0.36f + i * 0.14f), m + w * (i == 3 ? 0.55f : 0.8f), m + w * (0.36f + i * 0.14f));
    GdipDeletePen(pn);
    GdipDeleteGraphics(g);
    return b;
}

static void LoadItemIcon(Item *it) {
    if (it->icon) { GdipDisposeImage(it->icon); it->icon = NULL; }
    it->fav = 0;
    if (it->type == T_SEP) return;
    if (it->type == T_GROUP) { BuildGroupIcon(it); return; }
    int cs = g_cacheSize;
    if (it->type == T_NOTE) { it->icon = NoteIcon(cs); return; }
    if (it->icon_src[0] && GetFileAttributesW(it->icon_src) != INVALID_FILE_ATTRIBUTES) {
        GpBitmap *s = NULL;
        if (GdipCreateBitmapFromFile(it->icon_src, &s) == 0 && s) {
            it->icon = FitBitmap(s, cs, it->fit, 0.94f);
            GdipDisposeImage(s);
            if (it->icon) it->fav = 1;
        }
    }
    if (!it->icon) {
        if (it->type == T_URL) {
            WCHAR f[MAX_PATH], host[256];
            IconFileFor(it->path, f);
            if (GetFileAttributesW(f) != INVALID_FILE_ATTRIBUTES) {
                it->icon = FaviconBitmap(f, cs);
                if (it->icon) { if (!it->icon_src[0]) it->fav = 1; } else DeleteFileW(f);
            } else if (!g_noDl) StartDownload(0, it->path, f);
            if (!it->icon) { HostOf(it->path, host, 256); it->icon = LetterTile(host, cs); }
        } else if (it->type == T_TRASH) {
            LPITEMIDLIST pidl = NULL;
            if (SUCCEEDED(SHGetSpecialFolderLocation(NULL, CSIDL_BITBUCKET, &pidl)) && pidl) {
                it->icon = ShellIconBitmap(NULL, pidl, cs);
                CoTaskMemFree(pidl);
            }
        } else {
            if (it->type == T_FILE && (StrCmpNIW(it->path, L"shell:", 6) == 0 || StrCmpNIW(it->path, L"::{", 3) == 0)) {
                LPITEMIDLIST pidl = NULL;
                if (SUCCEEDED(SHParseDisplayName(it->path, NULL, &pidl, 0, NULL)) && pidl) { it->icon = ShellIconBitmap(NULL, pidl, cs); CoTaskMemFree(pidl); }
            } else {
                if (it->type == T_FILE && PathMatchSpecW(it->path, L"*.lnk")) it->icon = LnkIconBitmap(it->path, cs);
                if (!it->icon) it->icon = ShellIconBitmap(it->path, NULL, cs);
            }
        }
    }
    if (!it->icon) it->icon = LetterTile(it->name, cs);
}

static void ReloadAllIcons(void) {
    for (int i = 0; i < nItems; i++) {
        LoadItemIcon(&items[i]);
        for (int k = 0; k < items[i].nk; k++) LoadItemIcon(&items[i].kids[k]);
        if (items[i].type == T_GROUP) BuildGroupIcon(&items[i]);
    }
}

/* ======================= items ======================= */
static void FileDescr(const WCHAR *path, WCHAR *out, int cch) {
    out[0] = 0;
    DWORD h;
    DWORD sz = GetFileVersionInfoSizeW(path, &h);
    if (!sz) return;
    void *buf = LocalAlloc(LMEM_FIXED, sz);
    if (!buf) return;
    if (GetFileVersionInfoW(path, 0, sz, buf)) {
        struct { WORD l, c; } *tr = NULL;
        UINT len = 0;
        if (VerQueryValueW(buf, L"\\VarFileInfo\\Translation", (LPVOID *)&tr, &len) && len >= 4) {
            WCHAR q[96], *v = NULL;
            wsprintfW(q, L"\\StringFileInfo\\%04x%04x\\FileDescription", tr[0].l, tr[0].c);
            if (VerQueryValueW(buf, q, (LPVOID *)&v, &len) && len > 1) lstrcpynW(out, v, cch);
            if (!out[0]) {
                wsprintfW(q, L"\\StringFileInfo\\%04x%04x\\ProductName", tr[0].l, tr[0].c);
                if (VerQueryValueW(buf, q, (LPVOID *)&v, &len) && len > 1) lstrcpynW(out, v, cch);
            }
        }
    }
    LocalFree(buf);
}

static void CleanName(WCHAR *n) {
    for (WCHAR *p = n; *p; p++) if (*p == L'|' || *p == L'\r' || *p == L'\n') *p = L'/';
}

static void ResolveTarget(Item *it) {
    it->target[0] = 0;
    if (it->type != T_FILE) return;
    if (PathMatchSpecW(it->path, L"*.lnk")) {
        WCHAR t[MAX_PATH], a[1024], il[MAX_PATH];
        int ix;
        if (ReadLnk(it->path, t, a, il, &ix) && PathMatchSpecW(t, L"*.exe")) lstrcpynW(it->target, t, MAX_PATH);
    } else if (PathMatchSpecW(it->path, L"*.exe")) lstrcpynW(it->target, it->path, MAX_PATH);
}

static void FillItem(Item *it, int type, int appmode, const WCHAR *name, const WCHAR *path, const WCHAR *icon) {
    ZeroMemory(it, sizeof(Item));
    it->type = type;
    it->appmode = appmode;
    if (name) lstrcpynW(it->name, name, 128);
    if (path) lstrcpynW(it->path, path, 1024);
    if (icon) lstrcpynW(it->icon_src, icon, 512);
    CleanName(it->name);
    ResolveTarget(it);
    if (type != T_GROUP) LoadItemIcon(it);
}

static void FreeItem(Item *it) {
    if (it->icon) GdipDisposeImage(it->icon);
    if (it->kids) {
        for (int k = 0; k < it->nk; k++) if (it->kids[k].icon) GdipDisposeImage(it->kids[k].icon);
        HeapFree(GetProcessHeap(), 0, it->kids);
    }
    it->icon = NULL; it->kids = NULL; it->nk = 0;
}

static int PinnedCount(void) {
    for (int i = 0; i < nItems; i++) if (items[i].temp) return i;
    return nItems;
}

/* Insert an already built item (ownership of icon/kids moves into the array) */
static int InsertItemStruct(int idx, Item *src, BOOL temp) {
    if (nItems >= MAXI) { FreeItem(src); return -1; }
    int pc = PinnedCount();
    if (!temp && (idx < 0 || idx > pc)) idx = pc;
    if (temp && (idx < 0 || idx > nItems)) idx = nItems;
    for (int i = 0; i < nItems; i++) items[i].prevL = (i < idx) ? slotL[i] : slotL[i];
    memmove(&items[idx + 1], &items[idx], sizeof(Item) * (nItems - idx));
    nItems++;
    items[idx] = *src;
    items[idx].temp = temp;
    items[idx].cur = 0;
    items[idx].slide = 0;
    items[idx].prevL = slotL[idx < MAXI ? idx : 0];
    gMoved = TRUE;
    return idx;
}

static void RemoveItem(int i) {
    if (i < 0 || i >= nItems) return;
    FreeItem(&items[i]);
    memmove(&items[i], &items[i + 1], sizeof(Item) * (nItems - i - 1));
    nItems--;
    prevN = -1;
    ScheduleSave();
    Kick();
}

static void MoveItem(int from, int to) {
    if (from == to || from < 0 || to < 0 || from >= nItems || to >= nItems) return;
    for (int i = 0; i < nItems; i++) items[i].prevL = slotL[i];
    Item tmp = items[from];
    if (from < to) memmove(&items[from], &items[from + 1], sizeof(Item) * (to - from));
    else memmove(&items[to + 1], &items[to], sizeof(Item) * (from - to));
    items[to] = tmp;
    gMoved = TRUE;
}

static BOOL KidAdd(Item *g, Item *kid) {
    if (!g->kids) {
        g->kids = (Item *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(Item) * MAXK);
        if (!g->kids) return FALSE;
    }
    if (g->nk >= MAXK) return FALSE;
    kid->temp = 0; kid->cur = 0; kid->kids = NULL; kid->nk = 0;
    g->kids[g->nk++] = *kid;
    BuildGroupIcon(g);
    return TRUE;
}

static void MergeInto(int from, int to) {
    if (from == to || from < 0 || to < 0 || from >= nItems || to >= nItems) return;
    Item *a = &items[from], *t = &items[to];
    if (a->type == T_SEP || a->type == T_GROUP || t->type == T_SEP) return;
    Item dragged = *a;
    if (t->type == T_GROUP) {
        if (!KidAdd(t, &dragged)) return;
    } else {
        Item g;
        ZeroMemory(&g, sizeof g);
        g.type = T_GROUP;
        lstrcpyW(g.name, L"应用组");
        g.temp = t->temp;
        g.cur = t->cur;
        g.kids = (Item *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(Item) * MAXK);
        if (!g.kids) return;
        g.kids[0] = *t; g.kids[0].temp = 0; g.kids[0].cur = 0;
        g.kids[1] = dragged; g.kids[1].temp = 0; g.kids[1].cur = 0;
        g.nk = 2;
        BuildGroupIcon(&g);
        items[to] = g;
    }
    memmove(&items[from], &items[from + 1], sizeof(Item) * (nItems - from - 1));
    nItems--;
    prevN = -1;
    ScheduleSave();
}

static void NewEmptyGroup(void) {
    Item g;
    ZeroMemory(&g, sizeof g);
    g.type = T_GROUP;
    lstrcpyW(g.name, L"应用组");
    g.kids = (Item *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(Item) * MAXK);
    BuildGroupIcon(&g);
    InsertItemStruct(-1, &g, FALSE);
    ScheduleSave();
}

static void ExtractKid(int gi, int k) {
    if (gi < 0 || gi >= nItems || items[gi].type != T_GROUP || k < 0 || k >= items[gi].nk) return;
    Item kid = items[gi].kids[k];
    memmove(&items[gi].kids[k], &items[gi].kids[k + 1], sizeof(Item) * (items[gi].nk - k - 1));
    items[gi].nk--;
    BuildGroupIcon(&items[gi]);
    InsertItemStruct(gi + 1, &kid, items[gi].temp);
    ScheduleSave();
}

static void DissolveGroup(int gi) {
    if (gi < 0 || gi >= nItems || items[gi].type != T_GROUP) return;
    while (items[gi].nk > 0) ExtractKid(gi, items[gi].nk - 1);
    RemoveItem(gi);
}

/* Edge / Chrome installed web apps keep their icon under ...\Web Applications\_crx_<id>\*.ico */
static BOOL BuildItem(Item *it, const WCHAR *s0);
static BOOL FindPwaIcon(const WCHAR *args, WCHAR *out) {
    const WCHAR *p = wcsstr(args, L"--app-id=");
    if (!p) return FALSE;
    WCHAR id[40];
    int n = 0;
    p += 9;
    while (*p && *p != L' ' && *p != L'"' && n < 38) id[n++] = *p++;
    id[n] = 0;
    WCHAR local[MAX_PATH];
    if (FAILED(SHGetFolderPathW(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, local))) return FALSE;
    static const WCHAR *bases[2] = { L"Microsoft\\Edge\\User Data", L"Google\\Chrome\\User Data" };
    static const WCHAR *profs[4] = { L"Default", L"Profile 1", L"Profile 2", L"Profile 3" };
    for (int b = 0; b < 2; b++)
        for (int q = 0; q < 4; q++) {
            WCHAR pat[MAX_PATH * 2], dir[MAX_PATH * 2];
            wsprintfW(dir, L"%s\\%s\\%s\\Web Applications\\_crx_%s", local, bases[b], profs[q], id);
            wsprintfW(pat, L"%s\\*.ico", dir);
            WIN32_FIND_DATAW fd;
            HANDLE h = FindFirstFileW(pat, &fd);
            if (h != INVALID_HANDLE_VALUE) { wsprintfW(out, L"%s\\%s", dir, fd.cFileName); FindClose(h); return TRUE; }
        }
    return FALSE;
}

static BOOL BuildFromPidl(Item *it, LPCITEMIDLIST pidl) {
    PWSTR parse = NULL, disp = NULL;
    if (FAILED(SHGetNameFromIDList(pidl, SIGDN_DESKTOPABSOLUTEPARSING, &parse)) || !parse) return FALSE;
    SHGetNameFromIDList(pidl, SIGDN_NORMALDISPLAY, &disp);
    BOOL ok = FALSE;
    if (GetFileAttributesW(parse) != INVALID_FILE_ATTRIBUTES) ok = BuildItem(it, parse);
    else if (StrCmpNIW(parse, L"shell:", 6) == 0 || StrCmpNIW(parse, L"::{", 3) == 0) {
        FillItem(it, T_FILE, 0, disp ? disp : L"应用", parse, NULL);
        ok = TRUE;
    }
    if (parse) CoTaskMemFree(parse);
    if (disp) CoTaskMemFree(disp);
    return ok;
}

/* Build an item from a dropped path / url / shortcut. Returns FALSE when unusable. */
static BOOL BuildItem(Item *it, const WCHAR *s0) {
    WCHAR s[1024], name[128];
    lstrcpynW(s, s0, 1024);
    StrTrimW(s, L" \t\r\n\"");
    if (!s[0]) return FALSE;
    if (IsUrlStr(s)) {
        WCHAR url[1024];
        if (StrCmpNIW(s, L"www.", 4) == 0) wsprintfW(url, L"https://%s", s); else lstrcpyW(url, s);
        HostOf(url, name, 128);
        WCHAR *nm = name;
        if (StrCmpNIW(nm, L"www.", 4) == 0) nm += 4;
        FillItem(it, T_URL, cfg.webApp, nm, url, NULL);
        return TRUE;
    }
    if (StrCmpNIW(s, L"shell:", 6) == 0) { /* e.g. shell:AppsFolder\... from the Start menu */
        LPITEMIDLIST pidl = NULL;
        if (SUCCEEDED(SHParseDisplayName(s, NULL, &pidl, 0, NULL)) && pidl) {
            PWSTR disp = NULL;
            SHGetNameFromIDList(pidl, SIGDN_NORMALDISPLAY, &disp);
            FillItem(it, T_FILE, 0, disp ? disp : L"应用", s, NULL);
            if (disp) CoTaskMemFree(disp);
            CoTaskMemFree(pidl);
            return TRUE;
        }
        return FALSE;
    }
    DWORD at = GetFileAttributesW(s);
    if (at == INVALID_FILE_ATTRIBUTES) return FALSE;
    if (at & FILE_ATTRIBUTE_DIRECTORY) {
        WCHAR *b = PathFindFileNameW(s);
        lstrcpynW(name, (b && *b) ? b : s, 128);
        FillItem(it, T_FOLDER, 0, name, s, NULL);
        return TRUE;
    }
    BOOL isApp = PathMatchSpecW(s, L"*.exe;*.lnk;*.url;*.bat;*.cmd");
    lstrcpynW(name, PathFindFileNameW(s), 128);
    if (isApp) PathRemoveExtensionW(name);   /* documents keep their full file name, apps lose the extension */
    if (PathMatchSpecW(s, L"*.url")) {
        WCHAR url[1024] = L"";
        GetPrivateProfileStringW(L"InternetShortcut", L"URL", L"", url, 1024, s);
        if (url[0]) { FillItem(it, T_URL, cfg.webApp, name, url, NULL); return TRUE; }
    }
    WCHAR pwaIcon[MAX_PATH] = L"";
    if (PathMatchSpecW(s, L"*.lnk")) {
        WCHAR t[MAX_PATH], a[1024], il[MAX_PATH];
        int ix;
        if (ReadLnk(s, t, a, il, &ix)) {
            WCHAR *p = wcsstr(a, L"--app=");
            if (p && PathMatchSpecW(t, L"*.exe")) { /* browser shortcut with an explicit app URL */
                p += 6;
                WCHAR url[1024];
                int n = 0;
                BOOL q = (*p == L'"');
                if (q) p++;
                while (*p && n < 1023 && ((q && *p != L'"') || (!q && *p != L' '))) url[n++] = *p++;
                url[n] = 0;
                if (IsUrlStr(url)) { FillItem(it, T_URL, 1, name, url, NULL); return TRUE; }
            }
            if (!il[0]) FindPwaIcon(a, pwaIcon);   /* installed PWA: pick up its own icon */
        }
    }
    if (PathMatchSpecW(s, L"*.exe")) {
        WCHAR d[128];
        FileDescr(s, d, 128);
        if (d[0]) lstrcpynW(name, d, 128);
    }
    FillItem(it, T_FILE, 0, name, s, pwaIcon[0] ? pwaIcon : NULL);
    return TRUE;
}

static int AddFromString(const WCHAR *s, int idx) {
    Item it;
    if (!BuildItem(&it, s)) return -1;
    int r = InsertItemStruct(idx, &it, FALSE);
    if (r >= 0) ScheduleSave();
    return r;
}

static void AddNote(void) {
    for (int i = 0; i < nItems; i++) if (items[i].type == T_NOTE) return;
    Item it;
    FillItem(&it, T_NOTE, 0, L"速记", L"", NULL);
    InsertItemStruct(-1, &it, FALSE);
    ScheduleSave();
}
static void RemoveNote(void) {
    for (int i = nItems - 1; i >= 0; i--) if (items[i].type == T_NOTE) RemoveItem(i);
}

static void AddTrash(void) {
    Item it;
    FillItem(&it, T_TRASH, 0, L"回收站", L"shell:RecycleBinFolder", NULL);
    InsertItemStruct(-1, &it, FALSE);
    ScheduleSave();
}

/* ======================= save / load ======================= */
static int SplitFields(WCHAR *v, WCHAR **f, int max) {
    int n = 0;
    f[n++] = v;
    for (WCHAR *p = v; *p && n < max; p++)
        if (*p == L'|') { *p = 0; f[n++] = p + 1; }
    return n;
}

static void SaveNow(void) {
    EnsureIni();
    SaveSettingsOnly();
    WritePrivateProfileStringW(L"Items", NULL, NULL, g_ini);
    WritePrivateProfileStringW(L"Groups", NULL, NULL, g_ini);
    int si = 0;
    for (int i = 0; i < nItems; i++) {
        Item *it = &items[i];
        if (it->temp) continue;
        WCHAR k[24], v[2200];
        wsprintfW(k, L"I%d", si);
        wsprintfW(v, L"%d|%d|%s|%s|%s|%d|%s", it->type, it->appmode, it->name, it->path, it->icon_src, it->fit, it->args);
        WritePrivateProfileStringW(L"Items", k, v, g_ini);
        if (it->type == T_GROUP) {
            wsprintfW(k, L"N%d", si);
            wsprintfW(v, L"%d", it->nk);
            WritePrivateProfileStringW(L"Groups", k, v, g_ini);
            for (int j = 0; j < it->nk; j++) {
                Item *c = &it->kids[j];
                wsprintfW(k, L"G%d_%d", si, j);
                wsprintfW(v, L"%d|%d|%s|%s|%s|%d|%s", c->type, c->appmode, c->name, c->path, c->icon_src, c->fit, c->args);
                WritePrivateProfileStringW(L"Groups", k, v, g_ini);
            }
        }
        si++;
    }
    WCHAR b[24];
    wsprintfW(b, L"%d", si);
    WritePrivateProfileStringW(L"Items", L"Count", b, g_ini);
}

static BOOL ParseItemLine(const WCHAR *line, Item *it) {
    WCHAR v[2200], *f[7] = { 0 };
    lstrcpynW(v, line, 2200);
    int n = SplitFields(v, f, 7);
    if (n < 4) return FALSE;
    int type = _wtoi(f[0]);
    if (type < 0 || type > 6) return FALSE;
    int fit = n >= 6 ? _wtoi(f[5]) : 0;
    WCHAR *icon = (n >= 5 && f[4][0]) ? f[4] : NULL;
    if (fit != 0 && icon) { /* load once with the saved fit mode */
        ZeroMemory(it, sizeof(Item));
        it->fit = fit;
        it->type = type; it->appmode = _wtoi(f[1]);
        lstrcpynW(it->name, f[2], 128); lstrcpynW(it->path, f[3], 1024); lstrcpynW(it->icon_src, icon, 512);
        if (n >= 7) lstrcpynW(it->args, f[6], 260);
        ResolveTarget(it);
        LoadItemIcon(it);
        return TRUE;
    }
    FillItem(it, type, _wtoi(f[1]), f[2], f[3], icon);
    if (n >= 7) lstrcpynW(it->args, f[6], 260);
    return TRUE;
}

static void LoadItems(void) {
    int cnt = (int)GetPrivateProfileIntW(L"Items", L"Count", 0, g_ini);
    for (int i = 0; i < cnt && i < MAXI; i++) {
        WCHAR k[24], v[2200];
        wsprintfW(k, L"I%d", i);
        GetPrivateProfileStringW(L"Items", k, L"", v, 2200, g_ini);
        Item it;
        if (!ParseItemLine(v, &it)) continue;
        if (it.type == T_GROUP) {
            wsprintfW(k, L"N%d", i);
            int nk = (int)GetPrivateProfileIntW(L"Groups", k, 0, g_ini);
            for (int j = 0; j < nk && j < MAXK; j++) {
                Item kid;
                wsprintfW(k, L"G%d_%d", i, j);
                GetPrivateProfileStringW(L"Groups", k, L"", v, 2200, g_ini);
                if (ParseItemLine(v, &kid)) KidAdd(&it, &kid);
            }
            if (!it.kids) it.kids = (Item *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(Item) * MAXK);
            BuildGroupIcon(&it);
        }
        InsertItemStruct(-1, &it, FALSE);
    }
    gMoved = FALSE;
}

static void ReloadConfigAll(void) {
    for (int i = 0; i < nItems; i++) FreeItem(&items[i]);
    nItems = 0;
    prevN = -1;
    LoadConfig();
    g_cacheSize = 96;
    {
        int z = cfg.zoom ? cfg.zoomSize : cfg.iconSize;
        g_cacheSize = z <= 96 ? 96 : z <= 128 ? 128 : z <= 192 ? 192 : 256;
    }
    LoadItems();
    ApplySettings();
}

/* ---- export / import (.mdock = ini + icon cache in one file) ---- */
static BOOL ReadWhole(const WCHAR *f, BYTE **buf, DWORD *n) {
    HANDLE h = CreateFileW(f, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE) return FALSE;
    *n = GetFileSize(h, NULL);
    *buf = (BYTE *)HeapAlloc(GetProcessHeap(), 0, *n + 1);
    DWORD r = 0;
    BOOL ok = *buf && ReadFile(h, *buf, *n, &r, NULL) && r == *n;
    CloseHandle(h);
    if (!ok && *buf) HeapFree(GetProcessHeap(), 0, *buf);
    return ok;
}

static void WriteEntry(HANDLE h, const WCHAR *name, const WCHAR *file) {
    BYTE *b = NULL;
    DWORD n = 0, w;
    if (!ReadWhole(file, &b, &n)) return;
    DWORD nl = lstrlenW(name);
    WriteFile(h, &nl, 4, &w, NULL);
    WriteFile(h, name, nl * 2, &w, NULL);
    WriteFile(h, &n, 4, &w, NULL);
    WriteFile(h, b, n, &w, NULL);
    HeapFree(GetProcessHeap(), 0, b);
}

static void CopySection(const WCHAR *sec, const WCHAR *from, const WCHAR *to) {
    WCHAR *buf = (WCHAR *)HeapAlloc(GetProcessHeap(), 0, 65536 * sizeof(WCHAR));
    if (!buf) return;
    DWORD n = GetPrivateProfileSectionW(sec, buf, 65535, from);
    if (n > 0) WritePrivateProfileSectionW(sec, buf, to);
    HeapFree(GetProcessHeap(), 0, buf);
}

/* kind: 0 = everything, 1 = settings only, 2 = apps / groups / icons only */
static void ExportConfig(HWND parent, int kind) {
    SaveNow();
    WCHAR f[MAX_PATH];
    lstrcpyW(f, kind == 1 ? L"MiniDock设置参数.mdock" : kind == 2 ? L"MiniDock应用与图标.mdock" : L"MiniDock完整配置.mdock");
    OPENFILENAMEW o;
    ZeroMemory(&o, sizeof o);
    o.lStructSize = sizeof o; o.hwndOwner = parent;
    o.lpstrFilter = L"MiniDock 配置 (*.mdock)\0*.mdock\0";
    o.lpstrFile = f; o.nMaxFile = MAX_PATH; o.lpstrDefExt = L"mdock";
    o.Flags = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;
    if (!GetSaveFileNameW(&o)) return;
    WCHAR tmp[MAX_PATH];
    lstrcpyW(tmp, g_ini);
    lstrcatW(tmp, L".exp");
    DeleteFileW(tmp);
    { HANDLE h = CreateFileW(tmp, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
      if (h != INVALID_HANDLE_VALUE) { WORD bom = 0xFEFF; DWORD w; WriteFile(h, &bom, 2, &w, NULL); CloseHandle(h); } }
    if (kind != 2) CopySection(L"Settings", g_ini, tmp);
    if (kind != 1) { CopySection(L"Items", g_ini, tmp); CopySection(L"Groups", g_ini, tmp); }
    HANDLE h = CreateFileW(f, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE) { DeleteFileW(tmp); return; }
    DWORD w, cnt = 0;
    WIN32_FIND_DATAW fd;
    WCHAR pat[MAX_PATH], full[MAX_PATH], nm[MAX_PATH];
    PathCombineW(pat, g_iconDir, L"*");
    if (kind != 1) {
        HANDLE ff = FindFirstFileW(pat, &fd);
        if (ff != INVALID_HANDLE_VALUE) { do { if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) cnt++; } while (FindNextFileW(ff, &fd)); FindClose(ff); }
    }
    cnt++;
    WriteFile(h, "MDK1", 4, &w, NULL);
    WriteFile(h, &cnt, 4, &w, NULL);
    WriteEntry(h, L"MiniDock.ini", tmp);
    if (kind != 1) {
        HANDLE ff = FindFirstFileW(pat, &fd);
        if (ff != INVALID_HANDLE_VALUE) {
            do {
                if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue;
                PathCombineW(full, g_iconDir, fd.cFileName);
                wsprintfW(nm, L"icons\\%s", fd.cFileName);
                WriteEntry(h, nm, full);
            } while (FindNextFileW(ff, &fd));
            FindClose(ff);
        }
    }
    CloseHandle(h);
    DeleteFileW(tmp);
    MessageBoxW(parent, L"已导出。", L"MiniDock", MB_OK | MB_ICONINFORMATION);
}

static void ImportConfig(HWND parent) {
    WCHAR f[MAX_PATH] = L"";
    OPENFILENAMEW o;
    ZeroMemory(&o, sizeof o);
    o.lStructSize = sizeof o; o.hwndOwner = parent;
    o.lpstrFilter = L"MiniDock 配置 (*.mdock)\0*.mdock\0";
    o.lpstrFile = f; o.nMaxFile = MAX_PATH; o.Flags = OFN_FILEMUSTEXIST;
    if (!GetOpenFileNameW(&o)) return;
    BYTE *b = NULL;
    DWORD n = 0;
    if (!ReadWhole(f, &b, &n) || n < 8 || memcmp(b, "MDK1", 4) != 0) {
        MessageBoxW(parent, L"这不是有效的 MiniDock 配置文件。", L"MiniDock", MB_OK | MB_ICONWARNING);
        if (b) HeapFree(GetProcessHeap(), 0, b);
        return;
    }
    WCHAR imp[MAX_PATH];
    lstrcpyW(imp, g_ini);
    lstrcatW(imp, L".imp");
    DWORD cnt = *(DWORD *)(b + 4), pos = 8;
    for (DWORD i = 0; i < cnt && pos + 4 <= n; i++) {
        DWORD nl = *(DWORD *)(b + pos);
        pos += 4;
        if (nl > 260 || pos + nl * 2 + 4 > n) break;
        WCHAR nm[300];
        memcpy(nm, b + pos, nl * 2);
        nm[nl] = 0;
        pos += nl * 2;
        DWORD sz = *(DWORD *)(b + pos);
        pos += 4;
        if (pos + sz > n) break;
        WCHAR dst[MAX_PATH];
        if (!lstrcmpiW(nm, L"MiniDock.ini")) lstrcpyW(dst, imp);
        else PathCombineW(dst, g_iconDir, PathFindFileNameW(nm));
        HANDLE h = CreateFileW(dst, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (h != INVALID_HANDLE_VALUE) { DWORD w; WriteFile(h, b + pos, sz, &w, NULL); CloseHandle(h); }
        pos += sz;
    }
    HeapFree(GetProcessHeap(), 0, b);
    /* merge only the parts the file actually contains */
    CopySection(L"Settings", imp, g_ini);
    if ((int)GetPrivateProfileIntW(L"Items", L"Count", -1, imp) >= 0) {
        WritePrivateProfileStringW(L"Items", NULL, NULL, g_ini);
        WritePrivateProfileStringW(L"Groups", NULL, NULL, g_ini);
        CopySection(L"Items", imp, g_ini);
        CopySection(L"Groups", imp, g_ini);
    }
    DeleteFileW(imp);
    ReloadConfigAll();
    MessageBoxW(parent, L"已导入，图标已恢复。", L"MiniDock", MB_OK | MB_ICONINFORMATION);
}

/* ======================= geometry ======================= */
static HMONITOR g_mons[8];
static int g_nmon;
static BOOL CALLBACK MonEnum(HMONITOR m, HDC dc, LPRECT r, LPARAM l) { if (g_nmon < 8) g_mons[g_nmon++] = m; return TRUE; }

static void GetMon(void) {
    g_nmon = 0;
    EnumDisplayMonitors(NULL, NULL, MonEnum, 0);
    for (int i = 0; i < g_nmon; i++) {
        MONITORINFO mi = { sizeof mi };
        GetMonitorInfoW(g_mons[i], &mi);
        if (mi.dwFlags & MONITORINFOF_PRIMARY) { HMONITOR t = g_mons[0]; g_mons[0] = g_mons[i]; g_mons[i] = t; break; }
    }
    g_mi.cbSize = sizeof g_mi;
    if (g_nmon == 0) {
        GetMonitorInfoW(MonitorFromPoint((POINT){ 0, 0 }, MONITOR_DEFAULTTOPRIMARY), &g_mi);
        return;
    }
    GetMonitorInfoW((cfg.monitor < g_nmon) ? g_mons[cfg.monitor] : g_mons[0], &g_mi);
}

static float g_ce, g_reflH, g_accH = 210.f;

static void CalcMetrics(void) {
    float S0 = (float)cfg.iconSize;
    float Z0 = (float)(cfg.zoom ? (cfg.zoomSize > cfg.iconSize ? cfg.zoomSize : cfg.iconSize) : cfg.iconSize);
    float g0 = (float)cfg.spacing, pad0 = (float)cfg.pad, sep0 = 16.f + g0 * 0.5f, padX0 = pad0 + 2;
    int nIcon = 0, nSep = 0;
    for (int i = 0; i < nItems; i++) { if (items[i].type == T_SEP) nSep++; else nIcon++; }
    float wb = 2 * padX0 + nIcon * S0 + nSep * sep0 + (nItems > 1 ? g0 * (nItems - 1) : 0);
    if (nItems == 0) wb = 2 * padX0 + S0 * 4;
    RECT wa = g_mi.rcWork;
    float along = (float)(Horiz() ? wa.right - wa.left : wa.bottom - wa.top);
    float limit = along - 40, fit = 1;
    if (wb > limit) fit = Clampf((limit - 2 * padX0) / (wb - 2 * padX0), 0.25f, 1.f);
    M.S = S0 * fit; M.Z = Z0 * fit; M.g = g0 * fit; M.sepW = sep0 * fit;
    M.pad = pad0; M.padX = padX0;
    g_reflH = (cfg.reflect && cfg.edge == E_BOTTOM) ? M.S * 0.38f + 5 : 0;
    M.margin = (float)cfg.margin + g_reflH;
    M.barH = M.S + 2 * M.pad + 4;
    g_ce = cfg.clock ? (Horiz() ? (float)Sc(88) : (float)Sc(66)) + M.g + 6 : 0;
    g_clockW = (int)(g_ce - M.g - 6);
    float over = (M.Z - M.S - M.pad > 0 ? M.Z - M.S - M.pad : 0) + M.S * 0.55f + 34 + 8;
    if (!Horiz()) over += 170;
    M.overhang = over;
    float w = wb * fit + (M.Z - M.S) * 6 + M.S + 260 + g_ce;
    if (nItems == 0) w = wb + 260;
    if (w > along) w = along;
    M.lw = (int)w;
    M.lh = (int)(M.barH + over + M.margin) + 2;
}

static void FreeSurface(void) {
    if (g_g) { GdipDeleteGraphics(g_g); g_g = NULL; }
    if (g_surf) { GdipDisposeImage(g_surf); g_surf = NULL; }
    if (g_dib) { SelectObject(g_memdc, g_oldbmp); DeleteObject(g_dib); g_dib = NULL; }
    g_bits = NULL;
}

static void MakeSurface(int w, int h) {
    FreeSurface();
    BITMAPINFO bi;
    ZeroMemory(&bi, sizeof bi);
    bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bi.bmiHeader.biWidth = w; bi.bmiHeader.biHeight = -h; bi.bmiHeader.biPlanes = 1; bi.bmiHeader.biBitCount = 32;
    g_dib = CreateDIBSection(g_memdc, &bi, DIB_RGB_COLORS, (void **)&g_bits, NULL, 0);
    if (!g_dib) return;
    g_oldbmp = (HBITMAP)SelectObject(g_memdc, g_dib);
    GdipCreateBitmapFromScan0(w, h, w * 4, PXF_PARGB, g_bits, &g_surf);
    GdipGetImageGraphicsContext(g_surf, &g_g);
    SetupG(g_g);
}

/* ---- app bar: reserve desktop space so maximized windows avoid the dock ---- */
static BOOL g_abReg;
static RECT g_abRect;
static DWORD g_abKey;

static void SetupAppBar(BOOL on, int thick) {
    APPBARDATA ab;
    ZeroMemory(&ab, sizeof ab);
    ab.cbSize = sizeof ab;
    ab.hWnd = g_hwnd;
    if (!on) {
        if (g_abReg) { SHAppBarMessage(ABM_REMOVE, &ab); g_abReg = FALSE; g_abKey = 0; }
        return;
    }
    RECT m = g_mi.rcMonitor;
    DWORD key = (DWORD)(cfg.edge * 100003 + thick * 31 + m.left * 7 + m.top * 13 + m.right * 17 + m.bottom * 19) | 1;
    if (g_abReg && key == g_abKey) return;
    if (!g_abReg) { ab.uCallbackMessage = WM_APP_AB; SHAppBarMessage(ABM_NEW, &ab); g_abReg = TRUE; }
    RECT rc = m;
    switch (cfg.edge) {
    case E_TOP: ab.uEdge = ABE_TOP; rc.bottom = rc.top + thick; break;
    case E_LEFT: ab.uEdge = ABE_LEFT; rc.right = rc.left + thick; break;
    case E_RIGHT: ab.uEdge = ABE_RIGHT; rc.left = rc.right - thick; break;
    default: ab.uEdge = ABE_BOTTOM; rc.top = rc.bottom - thick; break;
    }
    ab.rc = rc;
    SHAppBarMessage(ABM_QUERYPOS, &ab);
    switch (cfg.edge) {
    case E_TOP: ab.rc.bottom = ab.rc.top + thick; break;
    case E_LEFT: ab.rc.right = ab.rc.left + thick; break;
    case E_RIGHT: ab.rc.left = ab.rc.right - thick; break;
    default: ab.rc.top = ab.rc.bottom - thick; break;
    }
    SHAppBarMessage(ABM_SETPOS, &ab);
    g_abRect = ab.rc;
    g_abKey = key;
}

static void UpdateGeometry(void) {
    GetMon();
    CalcMetrics();
    LW = M.lw; LH = M.lh;
    int ww = Horiz() ? LW : LH, hh = Horiz() ? LH : LW;
    if (ww != winW || hh != winH || !g_dib) { winW = ww; winH = hh; MakeSurface(winW, winH); }
    RECT wa = g_mi.rcWork;
    BOOL res = cfg.reserve && cfg.mode == 0 && g_hwnd;
    if (g_hwnd) SetupAppBar(res, (int)(M.barH + M.margin + 4));
    switch (cfg.edge) {
    case E_TOP:
        winX = wa.left + ((wa.right - wa.left) - winW) / 2; winY = res ? g_abRect.top : wa.top; break;
    case E_LEFT:
        winX = res ? g_abRect.left : wa.left; winY = wa.top + ((wa.bottom - wa.top) - winH) / 2; break;
    case E_RIGHT:
        winX = (res ? g_abRect.right : wa.right) - winW; winY = wa.top + ((wa.bottom - wa.top) - winH) / 2; break;
    default:
        winX = wa.left + ((wa.right - wa.left) - winW) / 2; winY = (res ? g_abRect.bottom : wa.bottom) - winH; break;
    }
}

/* ======================= frosted background ======================= */
static void BlurPass(BYTE *src, BYTE *dst, int w, int h, int r, BOOL horiz) {
    int len = horiz ? w : h, cnt = horiz ? h : w;
    int stepIn = horiz ? 4 : w * 4, stepLine = horiz ? w * 4 : 4;
    int win = 2 * r + 1;
    for (int line = 0; line < cnt; line++)
        for (int c = 0; c < 3; c++) {
            BYTE *s = src + (INT_PTR)line * stepLine + c, *d = dst + (INT_PTR)line * stepLine + c;
            int sum = 0;
            for (int k = -r; k <= r; k++) { int ix = k < 0 ? 0 : (k >= len ? len - 1 : k); sum += s[ix * stepIn]; }
            for (int i = 0; i < len; i++) {
                d[i * stepIn] = (BYTE)(sum / win);
                int a = i + r + 1, b = i - r;
                if (a >= len) a = len - 1;
                if (b < 0) b = 0;
                sum += s[a * stepIn] - s[b * stepIn];
            }
        }
}

static BOOL UpdateFrost(void) {
    if (cfg.bgMode != 2 || !g_dib || gFsHidden || !g_fdc) return FALSE;
    int sc = 4, w = winW / sc, h = winH / sc;
    if (w < 8) w = 8;
    if (h < 8) h = 8;
    if (w != g_fw || h != g_fh || !g_fbmp) {
        if (g_frost) { GdipDisposeImage(g_frost); g_frost = NULL; }
        if (g_fbmp) { SelectObject(g_fdc, g_fold); DeleteObject(g_fbmp); g_fbmp = NULL; }
        BITMAPINFO bi;
        ZeroMemory(&bi, sizeof bi);
        bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bi.bmiHeader.biWidth = w; bi.bmiHeader.biHeight = -h; bi.bmiHeader.biPlanes = 1; bi.bmiHeader.biBitCount = 32;
        g_fbmp = CreateDIBSection(g_fdc, &bi, DIB_RGB_COLORS, (void **)&g_fbits, NULL, 0);
        if (!g_fbmp) return FALSE;
        g_fold = (HBITMAP)SelectObject(g_fdc, g_fbmp);
        GdipCreateBitmapFromScan0(w, h, w * 4, PXF_PARGB, g_fbits, &g_frost);
        g_fw = w; g_fh = h;
    }
    HDC scr = GetDC(NULL);
    SetStretchBltMode(g_fdc, HALFTONE);
    SetBrushOrgEx(g_fdc, 0, 0, NULL);
    StretchBlt(g_fdc, 0, 0, w, h, scr, winX, winY, winW, winH, SRCCOPY);
    ReleaseDC(NULL, scr);
    GdiFlush();
    BYTE *tmp = (BYTE *)HeapAlloc(GetProcessHeap(), 0, (SIZE_T)w * h * 4);
    if (tmp) {
        int r = cfg.blur < 1 ? 1 : cfg.blur;
        for (int p = 0; p < 2; p++) {
            BlurPass(g_fbits, tmp, w, h, r, TRUE);
            BlurPass(tmp, g_fbits, w, h, r, FALSE);
        }
        HeapFree(GetProcessHeap(), 0, tmp);
    }
    DWORD hs = 2166136261u;
    {   /* true frosted glass: blurred backdrop + a light neutral tint, no glow */
        int tr = g_dark ? 26 : 248, tg = g_dark ? 27 : 249, tb = g_dark ? 32 : 252, tt = g_dark ? 105 : 140;
        for (int i = 0; i < w * h; i++) {
            BYTE *q = g_fbits + i * 4;
            q[0] = (BYTE)((q[0] * (255 - tt) + tb * tt) / 255);
            q[1] = (BYTE)((q[1] * (255 - tt) + tg * tt) / 255);
            q[2] = (BYTE)((q[2] * (255 - tt) + tr * tt) / 255);
            q[3] = 255;
            hs = (hs ^ (q[0] >> 2)) * 16777619u;
        }
    }
    BOOL ch = hs != g_fhash;
    g_fhash = hs;
    return ch;
}

/* ======================= running apps ======================= */
static WCHAR g_scanEx[48][MAX_PATH];
typedef struct { BOOL seen[MAXI]; int nex; } ScanCtx;
static int g_trashFull = -1;

static BOOL ExeOfWindow(HWND h, WCHAR *out) {
    DWORD pid = 0;
    GetWindowThreadProcessId(h, &pid);
    if (!pid) return FALSE;
    HANDLE p = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
    if (!p) return FALSE;
    DWORD n = MAX_PATH;
    BOOL ok = QueryFullProcessImageNameW(p, 0, out, &n);
    CloseHandle(p);
    return ok;
}

static BOOL IsAppWindow(HWND h) {
    if (!IsWindowVisible(h)) return FALSE;
    if (GetWindow(h, GW_OWNER)) return FALSE;
    if (GetWindowLongW(h, GWL_EXSTYLE) & WS_EX_TOOLWINDOW) return FALSE;
    if (GetWindowTextLengthW(h) == 0) return FALSE;
    int cloaked = 0;
    DwmGetWindowAttribute(h, 14, &cloaked, sizeof cloaked);
    if (cloaked) return FALSE;
    WCHAR cls[64];
    GetClassNameW(h, cls, 64);
    if (!lstrcmpW(cls, L"Progman") || !lstrcmpW(cls, L"WorkerW") || !lstrcmpW(cls, L"Shell_TrayWnd")) return FALSE;
    return TRUE;
}

static BOOL CALLBACK ScanProc(HWND h, LPARAM lp) {
    ScanCtx *c = (ScanCtx *)lp;
    if (!IsAppWindow(h) || h == g_hwnd || h == g_pv) return TRUE;
    WCHAR exe[MAX_PATH];
    if (!ExeOfWindow(h, exe)) return TRUE;
    BOOL known = FALSE;
    for (int i = 0; i < nItems; i++) {
        if (items[i].temp) continue;
        if (items[i].target[0] && lstrcmpiW(items[i].target, exe) == 0) { c->seen[i] = TRUE; known = TRUE; }
        for (int k = 0; k < items[i].nk; k++)
            if (items[i].kids[k].target[0] && lstrcmpiW(items[i].kids[k].target, exe) == 0) { c->seen[i] = TRUE; known = TRUE; }
    }
    if (known) return TRUE;
    const WCHAR *fn = PathFindFileNameW(exe);
    if (!lstrcmpiW(fn, L"ApplicationFrameHost.exe")) return TRUE;
    for (int k = 0; k < c->nex; k++) if (!lstrcmpiW(g_scanEx[k], exe)) return TRUE;
    if (c->nex < 48) lstrcpynW(g_scanEx[c->nex++], exe, MAX_PATH);
    return TRUE;
}

static BOOL SyncTemp(ScanCtx *c) {
    BOOL ch = FALSE;
    for (int i = nItems - 1; i >= 0; i--) {
        if (!items[i].temp || items[i].type == T_SEP) continue;
        BOOL f = FALSE;
        for (int k = 0; k < c->nex; k++) if (!lstrcmpiW(g_scanEx[k], items[i].target)) f = TRUE;
        if (!f || !cfg.showRunning) { RemoveItem(i); ch = TRUE; }
    }
    if (cfg.showRunning) {
        for (int k = 0; k < c->nex; k++) {
            BOOL have = FALSE;
            for (int i = 0; i < nItems; i++) if (items[i].temp && items[i].type != T_SEP && !lstrcmpiW(items[i].target, g_scanEx[k])) have = TRUE;
            if (have) continue;
            BOOL hasSep = FALSE;
            for (int i = 0; i < nItems; i++) if (items[i].temp && items[i].type == T_SEP) hasSep = TRUE;
            if (nItems + 2 > MAXI) break;
            if (!hasSep) { Item s; FillItem(&s, T_SEP, 0, L"", L"", NULL); InsertItemStruct(nItems, &s, TRUE); }
            WCHAR nm[128];
            FileDescr(g_scanEx[k], nm, 128);
            if (!nm[0]) { lstrcpynW(nm, PathFindFileNameW(g_scanEx[k]), 128); PathRemoveExtensionW(nm); }
            Item it;
            FillItem(&it, T_FILE, 0, nm, g_scanEx[k], NULL);
            it.running = TRUE;
            InsertItemStruct(nItems, &it, TRUE);
            ch = TRUE;
        }
    }
    int nt = 0, ns = 0;
    for (int i = 0; i < nItems; i++) if (items[i].temp) { if (items[i].type == T_SEP) ns++; else nt++; }
    if (nt == 0 && ns > 0) {
        for (int i = nItems - 1; i >= 0; i--) if (items[i].temp && items[i].type == T_SEP) RemoveItem(i);
        ch = TRUE;
    }
    return ch;
}

static void ScanRunning(void) {
    ScanCtx c;
    ZeroMemory(&c, sizeof c);
    EnumWindows(ScanProc, (LPARAM)&c);
    BOOL ch = SyncTemp(&c);
    for (int i = 0; i < nItems; i++) {
        BOOL r = items[i].temp ? TRUE : c.seen[i];
        if (items[i].running != r) { items[i].running = r; ch = TRUE; }
    }
    for (int i = 0; i < nItems; i++)
        if (items[i].type == T_TRASH) {
            SHQUERYRBINFO q;
            ZeroMemory(&q, sizeof q);
            q.cbSize = sizeof q;
            if (SUCCEEDED(SHQueryRecycleBinW(NULL, &q))) {
                int full = q.i64NumItems > 0;
                if (full != g_trashFull) { g_trashFull = full; LoadItemIcon(&items[i]); ch = TRUE; }
            }
        }
    BOOL oldDark = g_dark;
    RefreshAccent();
    UpdateTheme();
    if (oldDark != g_dark) ch = TRUE;
    if (UpdateFrost()) ch = TRUE;
    if (ch) Kick();
}

typedef struct { const WCHAR *exe; BOOL close; BOOL done; } ActCtx;
static BOOL CALLBACK ActProc(HWND h, LPARAM lp) {
    ActCtx *c = (ActCtx *)lp;
    if (!IsAppWindow(h) || h == g_hwnd) return TRUE;
    WCHAR exe[MAX_PATH];
    if (!ExeOfWindow(h, exe) || lstrcmpiW(exe, c->exe) != 0) return TRUE;
    if (c->close) { PostMessageW(h, WM_CLOSE, 0, 0); return TRUE; }
    if (IsIconic(h)) ShowWindow(h, SW_RESTORE);
    SetForegroundWindow(h);
    c->done = TRUE;
    return FALSE;
}
static BOOL ActivateApp(const WCHAR *exe, BOOL close) {
    ActCtx c = { exe, close, FALSE };
    EnumWindows(ActProc, (LPARAM)&c);
    return c.done;
}

static BOOL IsFullscreenForeground(void) {
    HWND f = GetForegroundWindow();
    if (!f || f == g_hwnd || f == g_hset || f == GetDesktopWindow() || f == GetShellWindow()) return FALSE;
    WCHAR cls[64];
    GetClassNameW(f, cls, 64);
    if (!lstrcmpW(cls, L"Progman") || !lstrcmpW(cls, L"WorkerW") || !lstrcmpW(cls, L"Shell_TrayWnd")) return FALSE;
    RECT r;
    GetWindowRect(f, &r);
    MONITORINFO mi = { sizeof mi };
    GetMonitorInfoW(MonitorFromWindow(f, MONITOR_DEFAULTTONEAREST), &mi);
    if (!(r.left <= mi.rcMonitor.left && r.top <= mi.rcMonitor.top && r.right >= mi.rcMonitor.right && r.bottom >= mi.rcMonitor.bottom)) return FALSE;
    return (GetWindowLongW(f, GWL_STYLE) & WS_CAPTION) != WS_CAPTION;
}

/* does any normal window cover the dock area? (smart hide) */
typedef struct { RECT bar; BOOL hit; } OvCtx;
static BOOL CALLBACK OvProc(HWND h, LPARAM lp) {
    OvCtx *c = (OvCtx *)lp;
    if (h == g_hwnd || h == g_hset || h == g_pv || !IsWindowVisible(h) || IsIconic(h)) return TRUE;
    if (GetWindowLongW(h, GWL_EXSTYLE) & (WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE)) return TRUE;
    if (GetWindow(h, GW_OWNER) || GetWindowTextLengthW(h) == 0) return TRUE;
    int cloaked = 0;
    DwmGetWindowAttribute(h, 14, &cloaked, sizeof cloaked);
    if (cloaked) return TRUE;
    WCHAR cls[64];
    GetClassNameW(h, cls, 64);
    if (!lstrcmpW(cls, L"Progman") || !lstrcmpW(cls, L"WorkerW") || !lstrcmpW(cls, L"Shell_TrayWnd")) return TRUE;
    RECT r, o;
    if (!GetWindowRect(h, &r)) return TRUE;
    if (IntersectRect(&o, &r, &c->bar)) { c->hit = TRUE; return FALSE; }
    return TRUE;
}

/* ======================= launching ======================= */
static void OpenUrlItem(Item *it) {
    WCHAR args[1100];
    if (it->appmode) {
        wsprintfW(args, L"--app=%s", it->path);
        if ((INT_PTR)ShellExecuteW(NULL, L"open", L"msedge.exe", args, NULL, SW_SHOWNORMAL) > 32) return;
        if ((INT_PTR)ShellExecuteW(NULL, L"open", L"chrome.exe", args, NULL, SW_SHOWNORMAL) > 32) return;
    }
    ShellExecuteW(NULL, L"open", it->path, NULL, NULL, SW_SHOWNORMAL);
}

static void LaunchPtr(Item *it) {
    if (it->type == T_URL) OpenUrlItem(it);
    else if (it->type == T_TRASH) ShellExecuteW(NULL, L"open", L"shell:RecycleBinFolder", NULL, NULL, SW_SHOWNORMAL);
    else {
        WCHAR dir[MAX_PATH];
        lstrcpynW(dir, it->path, MAX_PATH);
        if (it->type == T_FILE) PathRemoveFileSpecW(dir); else dir[0] = 0;
        ShellExecuteW(NULL, L"open", it->path, it->args[0] ? it->args : NULL, dir[0] ? dir : NULL, SW_SHOWNORMAL);
    }
}

static void LaunchItem(int i, BOOL force) {
    if (i < 0 || i >= nItems) return;
    Item *it = &items[i];
    if (it->type == T_SEP) return;
    if (it->type == T_GROUP) { ShowGroup(i); return; }
    if (it->type == T_NOTE) { ShowNote(); return; }
    if (it->type == T_FOLDER && cfg.folderFan && !force) { ShowFan(i); return; }
    if (it->type == T_FILE && it->target[0] && it->running && !force && ActivateApp(it->target, FALSE)) return;
    if (cfg.bounce) it->bounce = GetTickCount() | 1;
    LaunchPtr(it);
    SetTimer(g_hwnd, T_SCAN, 1200, NULL);
    Kick();
}

/* ======================= rendering ======================= */
static float MapActualToBase(float mx) {
    if (prevN != nItems || nItems == 0) return mx;
    if (nItems == 1) return baseC[0] + (mx - actC[0]);
    if (mx <= actC[0]) return baseC[0] + (mx - actC[0]);
    if (mx >= actC[nItems - 1]) return baseC[nItems - 1] + (mx - actC[nItems - 1]);
    for (int i = 0; i < nItems - 1; i++)
        if (mx >= actC[i] && mx < actC[i + 1]) {
            float d = actC[i + 1] - actC[i];
            float t = d > 0.001f ? (mx - actC[i]) / d : 0;
            return baseC[i] + t * (baseC[i + 1] - baseC[i]);
        }
    return mx;
}

static int HitIndex(float u, float v) {
    if (gHideCur > 0.5f) return -1;
    if (v < barTop - M.Z - 16 || v > LH) return -1;
    for (int i = 0; i < nItems; i++)
        if (items[i].cur > 1 && u >= slotL[i] - M.g / 2 && u < slotR[i] + M.g / 2) return i;
    return -1;
}

static float BounceOff(Item *it, BOOL *active) {
    if (!it->bounce) return 0;
    DWORD t = GetTickCount() - it->bounce;
    if (t > 1100) { it->bounce = 0; return 0; }
    *active = TRUE;
    float x = t / 1100.f;
    return fabsf(sinf(x * PI_F * 2.f)) * (1.f - x * 0.55f) * M.S * 0.48f;
}

static void DrawBubbleAt(GpGraphics *g, const WCHAR *text, float ix, float iy, float iw, float ih) {
    float w = TextWidth(g, text, g_font) + 24, h = 28, x, y;
    switch (cfg.edge) {
    case E_TOP: x = ix + iw / 2 - w / 2; y = iy + ih + 8; break;
    case E_LEFT: x = ix + iw + 10; y = iy + ih / 2 - h / 2; break;
    case E_RIGHT: x = ix - 10 - w; y = iy + ih / 2 - h / 2; break;
    default: x = ix + iw / 2 - w / 2; y = iy - 8 - h; break;
    }
    if (x < 4) x = 4;
    if (x + w > winW - 4) x = winW - 4 - w;
    if (y < 4) y = 4;
    if (y + h > winH - 4) y = winH - 4 - h;
    FillRR(g, ARGBC(228, 38, 38, 42), x, y, w, h, 9);
    StrokeRR(g, ARGBC(50, 255, 255, 255), 1, x, y, w, h, 9);
    DrawCentered(g, text, g_font, x, y, w, h, ARGBC(255, 245, 245, 247));
}

static void RefreshAccent(void) {
    DWORD col = 0;
    BOOL op = FALSE;
    if (SUCCEEDED(DwmGetColorizationColor(&col, &op))) {
        float r = ((col >> 16) & 255) / 255.f, g = ((col >> 8) & 255) / 255.f, b = (col & 255) / 255.f;
        float mx = r > g ? (r > b ? r : b) : (g > b ? g : b), mn = r < g ? (r < b ? r : b) : (g < b ? g : b), d = mx - mn, h = 0;
        if (d > 0.001f) {
            if (mx == r) h = fmodf((g - b) / d, 6.f); else if (mx == g) h = (b - r) / d + 2; else h = (r - g) / d + 4;
            h *= 60; if (h < 0) h += 360;
        }
        g_accH = h;
    }
}

/* bar / clock background: solid, gradient (Material-You tones) or frosted glass; opacity 0 = nothing at all */
static void DrawBar(GpGraphics *G_, float bx, float by, float bw, float bh, float rad, float tx, float ty) {
    if (cfg.opacity <= 0) return;
    float op = cfg.opacity / 100.f;
    if (cfg.bgMode == 2 && g_frost) {
        GpPath *cp;
        GdipCreatePath(0, &cp);
        DrawRoundRectPath(cp, bx, by, bw, bh, rad);
        GdipSetClipPath(G_, cp, 0);
        float cm[25] = { 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, op, 0, 0, 0, 0, 0, 1 };
        GdipSetImageAttributesColorMatrix(g_attrFrost, 1, TRUE, cm, NULL, 0);
        UINT sw = 0, sh = 0;
        GdipGetImageWidth(g_frost, &sw);
        GdipGetImageHeight(g_frost, &sh);
        GdipDrawImageRectRect(G_, g_frost, -tx, -ty, (float)winW, (float)winH, 0, 0, (REAL)sw, (REAL)sh, 2, g_attrFrost, NULL, NULL);
        GdipResetClip(G_);
        GdipDeletePath(cp);
    } else {
        int c1r, c1g, c1b, c2r, c2g, c2b;
        if (cfg.accentTone) {
            if (g_dark) { HsvRgb(g_accH, 0.42f, 0.34f, &c1r, &c1g, &c1b); HsvRgb(fmodf(g_accH + 18, 360), 0.5f, 0.22f, &c2r, &c2g, &c2b); }
            else { HsvRgb(g_accH, 0.16f, 0.97f, &c1r, &c1g, &c1b); HsvRgb(fmodf(g_accH + 18, 360), 0.24f, 0.90f, &c2r, &c2g, &c2b); }
        } else if (cfg.bgColor1 >= 0) {
            c1r = GetRValue(cfg.bgColor1); c1g = GetGValue(cfg.bgColor1); c1b = GetBValue(cfg.bgColor1);
            if (cfg.bgColor2 >= 0) { c2r = GetRValue(cfg.bgColor2); c2g = GetGValue(cfg.bgColor2); c2b = GetBValue(cfg.bgColor2); }
            else { c2r = c1r * 3 / 4; c2g = c1g * 3 / 4; c2b = c1b * 3 / 4; }
        } else if (g_dark) { c1r = 58; c1g = 60; c1b = 68; c2r = 30; c2g = 31; c2b = 36; }
        else { c1r = 255; c1g = 255; c1b = 255; c2r = 232; c2g = 235; c2b = 243; }
        int al = (int)(op * 255);
        RectF br = { bx, by, bw, bh };
        GpBrush *bb;
        if (cfg.bgMode == 0) GdipCreateSolidFill(ARGBC(al, c1r, c1g, c1b), &bb);
        else GdipCreateLineBrushFromRect(&br, ARGBC(al, c1r, c1g, c1b), ARGBC(al, c2r, c2g, c2b), Horiz() ? 2 : 1, 0, &bb);
        FillRoundRect(G_, bb, bx, by, bw, bh, rad);
        GdipDeleteBrush(bb);
    }
    if (cfg.border) {
        int ba = (int)((g_dark ? 90 : 150) * (op < 0.35f ? op / 0.35f : 1.f));
        StrokeRR(G_, ARGBC(ba, 255, 255, 255), 1.2f, bx, by, bw, bh, rad);
    }
}

static void DrawReflection(GpGraphics *G_, GpBitmap *ic, float lx, float ry, float sz) {
    UINT sw = 0, sh = 0;
    GdipGetImageWidth(ic, &sw);
    GdipGetImageHeight(ic, &sh);
    float rows = sh * 0.38f, half = rows / 2, rh = sz * 0.38f;
    for (int k = 0; k < 2; k++) {
        float srcY = (float)sh - half * (k + 1), dy = ry + (rh / 2) * k;
        float pts[6] = { lx, dy + rh / 2, lx + sz, dy + rh / 2, lx, dy };   /* vertical flip */
        GdipDrawImagePointsRect(G_, ic, pts, 3, 0, srcY, (REAL)sw, half, 2, k == 0 ? g_attrRefl : g_attrRefl2, NULL, NULL);
    }
}

static BOOL RenderFrame(void) {
    if (!g_hwnd || !g_g) return FALSE;
    CalcMetrics();
    if (M.lw != LW || M.lh != LH) UpdateGeometry();
    int n = nItems;
    BOOL anim = FALSE;
    float S = M.S, Z = M.Z, g = M.g, sepW = M.sepW, padX = M.padX, pad = M.pad;

    float ht = (float)(gForceHide ? 1 : gHideTgt);
    if (fabsf(ht - gHideCur) > 0.004f) { gHideCur += (ht - gHideCur) * 0.28f; anim = TRUE; } else gHideCur = ht;

    /* base (unzoomed) layout */
    float baseW = 2 * padX;
    for (int i = 0; i < n; i++) baseW += (items[i].type == T_SEP ? sepW : S);
    if (n > 1) baseW += g * (n - 1);
    if (n == 0) baseW = 2 * padX + S * 4;
    float bl = (LW - baseW - g_ce) / 2, x = bl + padX;
    for (int i = 0; i < n; i++) {
        float w = items[i].type == T_SEP ? sepW : S;
        baseC[i] = x + w / 2;
        x += w + g;
    }

    BOOL mag = cfg.zoom && hover && dragIdx < 0 && gapIdx < 0 && n > 0 && mouseU >= 0 && gHideCur < 0.5f;
    float u = mag ? MapActualToBase(mouseU) : 0;
    float R = (S + g) * 2.6f;
    for (int i = 0; i < n; i++) {
        Item *it = &items[i];
        float tgt;
        if (it->type == T_SEP) tgt = sepW;
        else {
            float w = 0;
            if (mag) { float d = fabsf(baseC[i] - u); if (d < R) w = 0.5f * (1.f + cosf(PI_F * d / R)); }
            tgt = S + (Z - S) * w;
        }
        if (i == dragIdx) tgt = removing ? 0.f : (it->type == T_SEP ? sepW : S);
        float d = tgt - it->cur;
        if (fabsf(d) > 0.3f) { it->cur += d * 0.38f; anim = TRUE; } else it->cur = tgt;
    }
    float gapTgt = gapIdx >= 0 ? S : 0;
    if (fabsf(gapTgt - gapCur) > 0.3f) { gapCur += (gapTgt - gapCur) * 0.4f; anim = TRUE; } else gapCur = gapTgt;
    float G = (gapCur > 0.5f) ? gapCur + (n > 0 ? g : 0) : 0;

    float tot = 2 * padX + G;
    for (int i = 0; i < n; i++) tot += items[i].cur;
    if (n > 1) tot += g * (n - 1);
    if (n == 0) tot += S * 4;
    barW = tot;
    if (barW > LW - 4 - g_ce) barW = LW - 4 - g_ce;
    barL = (LW - barW - g_ce) / 2;
    x = barL + padX;
    for (int i = 0; i < n; i++) {
        if (gapIdx == i && G > 0) x += G;
        slotL[i] = x;
        slotR[i] = x + items[i].cur;
        actC[i] = (slotL[i] + slotR[i]) / 2;
        x += items[i].cur + g;
    }
    prevN = n;
    if (gMoved) {
        for (int i = 0; i < n; i++) items[i].slide += items[i].prevL - slotL[i];
        gMoved = FALSE;
    }
    for (int i = 0; i < n; i++) {
        if (fabsf(items[i].slide) > 0.5f) { items[i].slide *= 0.62f; anim = TRUE; } else items[i].slide = 0;
    }
    barBottom = (float)LH - M.margin - 2;
    barTop = barBottom - M.barH;
    hot = (hover && dragIdx < 0) ? HitIndex(mouseU, mouseV) : -1;   /* after layout so the label always matches the icon */

    float iconBottom = barBottom - pad - 2;
    float off = gHideCur * (M.barH + M.margin + 14), tx = 0, ty = 0;
    switch (cfg.edge) { case E_TOP: ty = -off; break; case E_LEFT: tx = -off; break; case E_RIGHT: tx = off; break; default: ty = off; }
    GpGraphics *G_ = g_g;

    GdipGraphicsClear(G_, 0);
    if (gHideCur < 0.995f) {
        GdipResetWorldTransform(G_);
        GdipTranslateWorldTransform(G_, tx, ty, 0);

        float bx, by, bw, bh;
        MapR(barL, barTop, barW, M.barH, &bx, &by, &bw, &bh);
        float rad = (float)cfg.radius;
        if (rad > M.barH / 2) rad = M.barH / 2;
        DrawBar(G_, bx, by, bw, bh, rad, tx, ty);

        if (n == 0) DrawCentered(G_, L"将应用 / 文件夹 / 网址拖到这里", g_font, bx, by, bw, bh, g_dark ? ARGBC(190, 255, 255, 255) : ARGBC(190, 40, 40, 40));

        /* clock + date card */
        if (cfg.clock) {
            float cx, cy, cw, ch;
            MapR(barL + barW + g + 6, barTop, (float)g_clockW, M.barH, &cx, &cy, &cw, &ch);
            DrawBar(G_, cx, cy, cw, ch, rad, tx, ty);
            SYSTEMTIME st;
            GetLocalTime(&st);
            WCHAR tm[16], dt[32];
            static const WCHAR *wk[7] = { L"日", L"一", L"二", L"三", L"四", L"五", L"六" };
            wsprintfW(tm, L"%02d:%02d", st.wHour, st.wMinute);
            wsprintfW(dt, L"%d月%d日 周%s", st.wMonth, st.wDay, wk[st.wDayOfWeek]);
            ARGB tc = g_dark ? ARGBC(245, 240, 240, 245) : ARGBC(245, 30, 30, 36);
            DrawCentered(G_, tm, g_fontBold, cx, cy + ch * 0.12f, cw, ch * 0.44f, tc);
            DrawCentered(G_, dt, g_font, cx, cy + ch * 0.54f, cw, ch * 0.32f, g_dark ? ARGBC(190, 220, 220, 228) : ARGBC(190, 60, 60, 70));
        }

        /* separators */
        GpPen *sp;
        GdipCreatePen1(g_dark ? ARGBC(110, 255, 255, 255) : ARGBC(120, 60, 60, 70), 1.4f, 2, &sp);
        for (int i = 0; i < n; i++) {
            if (items[i].type != T_SEP || items[i].cur < 2 || i == dragIdx) continue;
            float cx = actC[i] + items[i].slide, lx, ly, lw, lh;
            MapR(cx, iconBottom - S * 0.88f, 0, S * 0.84f, &lx, &ly, &lw, &lh);
            GdipDrawLine(G_, sp, lx, ly, lx + lw, ly + lh);
        }
        GdipDeletePen(sp);

        if (mergeT >= 0 && mergeT < n && items[mergeT].cur > 1) {
            float hx, hy, hw, hh;
            MapR(slotL[mergeT] - 5, iconBottom - items[mergeT].cur - 5, items[mergeT].cur + 10, items[mergeT].cur + 10, &hx, &hy, &hw, &hh);
            FillRR(G_, ARGBC(70, 255, 255, 255), hx, hy, hw, hh, hw * 0.24f);
            StrokeRR(G_, ARGBC(190, 255, 255, 255), 2.f, hx, hy, hw, hh, hw * 0.24f);
        }

        /* icons (optional plate behind each icon) */
        int plate = g_dark ? cfg.plateDark : cfg.plateLight;
        BOOL bAct = FALSE;
        for (int i = 0; i < n; i++) {
            Item *it = &items[i];
            if (it->type == T_SEP || i == dragIdx || it->cur < 1 || !it->icon) continue;
            float bo = BounceOff(it, &bAct), sz = it->cur, lx = slotL[i] + it->slide;
            float ix, iy, iw, ih;
            MapR(lx, iconBottom - sz - bo, sz, sz, &ix, &iy, &iw, &ih);
            if (plate > 0 && it->type != T_GROUP && it->type != T_NOTE) {
                ARGB pc = plate == 1 ? ARGBC(255, 244, 245, 249) : plate == 2 ? ARGBC(255, 48, 50, 58) : plate == 3 ? ARGBC(120, 255, 255, 255) : ARGBC(130, 18, 19, 24);
                float pr = sz * cfg.plateRadius / 100.f;
                if (pr > sz / 2) pr = sz / 2;
                FillRR(G_, pc, ix, iy, iw, ih, pr);
                if (plate >= 3) StrokeRR(G_, ARGBC(60, 255, 255, 255), 1.f, ix, iy, iw, ih, pr);
                float in = sz * 0.13f;
                DrawImg(G_, it->icon, ix + in, iy + in, iw - 2 * in, ih - 2 * in, FALSE);
            } else DrawImg(G_, it->icon, ix, iy, iw, ih, FALSE);
            if (g_reflH > 0 && !bAct) DrawReflection(G_, it->icon, lx, barBottom + 3, sz);
            if (it->running && cfg.dots) {
                /* clearly visible indicator: pill (or dot) with a contrasting outline */
                float ph = S / 13.f, pw = cfg.dotStyle == 0 ? S * 0.32f : S / 11.f;
                if (ph < 3.4f) ph = 3.4f;
                if (cfg.dotStyle != 0) ph = pw;
                if (pw < 6) pw = 6;
                float dx, dy, dw, dh;
                MapR(lx + sz / 2 - pw / 2, barBottom - 2.5f - ph, pw, ph, &dx, &dy, &dw, &dh);
                float rr = (dw < dh ? dw : dh) / 2;
                FillRR(G_, g_dark ? ARGBC(255, 255, 255, 255) : ARGBC(255, 24, 26, 34), dx, dy, dw, dh, rr);
                StrokeRR(G_, g_dark ? ARGBC(120, 0, 0, 0) : ARGBC(150, 255, 255, 255), 1.f, dx - 0.5f, dy - 0.5f, dw + 1, dh + 1, rr);
            }
        }
        if (bAct) anim = TRUE;

        if (hot >= 0 && cfg.labels && !g_pv && items[hot].type != T_SEP && hover && gapIdx < 0 && items[hot].cur > 1 && !gMenuOpen) {
            float ix, iy, iw, ih;
            MapR(slotL[hot], iconBottom - items[hot].cur, items[hot].cur, items[hot].cur, &ix, &iy, &iw, &ih);
            DrawBubbleAt(G_, items[hot].name, ix, iy, iw, ih);
        }

        GdipResetWorldTransform(G_);
        if (dragIdx >= 0 && dragIdx < n) {
            if (items[dragIdx].type == T_SEP) {   /* ghost line so separators can be dragged too */
                GpPen *gp;
                GdipCreatePen1(g_dark ? ARGBC(220, 255, 255, 255) : ARGBC(220, 40, 40, 50), 2.f, 2, &gp);
                float hl = S * 0.42f;
                if (Horiz()) GdipDrawLine(G_, gp, (REAL)dragPtA.x, dragPtA.y - hl, (REAL)dragPtA.x, dragPtA.y + hl);
                else GdipDrawLine(G_, gp, dragPtA.x - hl, (REAL)dragPtA.y, dragPtA.x + hl, (REAL)dragPtA.y);
                GdipDeletePen(gp);
            } else if (items[dragIdx].icon) {
                float sz = S * 1.15f;
                DrawImg(G_, items[dragIdx].icon, dragPtA.x - sz / 2, dragPtA.y - sz * 0.6f, sz, sz, TRUE);
                if (removing) DrawBubbleAt(G_, L"移除", dragPtA.x - sz / 2, dragPtA.y - sz * 0.6f, sz, sz);
            }
        }
    }

    POINT pt = { winX, winY }, src = { 0, 0 };
    SIZE sz = { winW, winH };
    BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
    UpdateLayeredWindow(g_hwnd, NULL, &pt, &sz, g_memdc, &src, 0, &bf, ULW_ALPHA);
    return anim;
}

static void Kick(void) {
    if (!g_hwnd) return;
    if (!gAnimOn) { gAnimOn = TRUE; SetTimer(g_hwnd, T_ANIM, 15, NULL); }
    KillTimer(g_hwnd, T_TRIM);
}

/* screen rect of an icon (for popups) */
static void IconScreenRect(int idx, RECT *rc) {
    float iconBottom = barBottom - M.pad - 2;
    float ix, iy, iw, ih;
    MapR(slotL[idx], iconBottom - items[idx].cur, items[idx].cur, items[idx].cur, &ix, &iy, &iw, &ih);
    rc->left = winX + (int)ix; rc->top = winY + (int)iy;
    rc->right = rc->left + (int)iw; rc->bottom = rc->top + (int)ih;
}

/* ======================= input box ======================= */
typedef struct {
    HWND hw, e1, e2;
    const WCHAR *title, *l1, *l2;
    WCHAR *b1, *b2;
    int n1, n2, ok, done;
} InCtx;

static void SetFontAll(HWND parent) {
    for (HWND c = GetWindow(parent, GW_CHILD); c; c = GetWindow(c, GW_HWNDNEXT)) SendMessageW(c, WM_SETFONT, (WPARAM)g_uiFont, TRUE);
}

static LRESULT CALLBACK InProc(HWND h, UINT m, WPARAM w, LPARAM l) {
    InCtx *c = (InCtx *)GetWindowLongPtrW(h, GWLP_USERDATA);
    switch (m) {
    case WM_CREATE: {
        c = (InCtx *)((CREATESTRUCTW *)l)->lpCreateParams;
        SetWindowLongPtrW(h, GWLP_USERDATA, (LONG_PTR)c);
        c->hw = h;
        int y = Sc(14), W = Sc(420);
        CreateWindowW(L"STATIC", c->l1, WS_CHILD | WS_VISIBLE, Sc(16), y, W - Sc(32), Sc(20), h, NULL, g_inst, NULL);
        y += Sc(24);
        c->e1 = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", c->b1, WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL, Sc(16), y, W - Sc(32), Sc(26), h, (HMENU)1001, g_inst, NULL);
        y += Sc(36);
        if (c->l2) {
            CreateWindowW(L"STATIC", c->l2, WS_CHILD | WS_VISIBLE, Sc(16), y, W - Sc(32), Sc(20), h, NULL, g_inst, NULL);
            y += Sc(24);
            c->e2 = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", c->b2, WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL, Sc(16), y, W - Sc(32), Sc(26), h, (HMENU)1002, g_inst, NULL);
            y += Sc(36);
        }
        CreateWindowW(L"BUTTON", L"确定", WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_DEFPUSHBUTTON, W - Sc(188), y, Sc(80), Sc(28), h, (HMENU)IDOK, g_inst, NULL);
        CreateWindowW(L"BUTTON", L"取消", WS_CHILD | WS_VISIBLE | WS_TABSTOP, W - Sc(100), y, Sc(80), Sc(28), h, (HMENU)IDCANCEL, g_inst, NULL);
        SetFontAll(h);
        SetFocus(c->e1);
        SendMessageW(c->e1, EM_SETSEL, 0, -1);
        return 0;
    }
    case WM_COMMAND:
        if (LOWORD(w) == IDOK) {
            GetWindowTextW(c->e1, c->b1, c->n1);
            if (c->e2) GetWindowTextW(c->e2, c->b2, c->n2);
            c->ok = 1; c->done = 1;
            DestroyWindow(h);
        } else if (LOWORD(w) == IDCANCEL) { c->done = 1; DestroyWindow(h); }
        return 0;
    case WM_CLOSE: c->done = 1; DestroyWindow(h); return 0;
    }
    return DefWindowProcW(h, m, w, l);
}

static BOOL InputBox(HWND parent, const WCHAR *title, const WCHAR *l1, WCHAR *b1, int n1, const WCHAR *l2, WCHAR *b2, int n2) {
    InCtx c;
    ZeroMemory(&c, sizeof c);
    c.title = title; c.l1 = l1; c.l2 = l2; c.b1 = b1; c.b2 = b2; c.n1 = n1; c.n2 = n2;
    int W = Sc(420), H = Sc(l2 ? 200 : 136);
    RECT wa = g_mi.rcWork;
    RECT r = { 0, 0, W, H };
    AdjustWindowRectEx(&r, WS_CAPTION | WS_SYSMENU, FALSE, WS_EX_TOPMOST);
    HWND h = CreateWindowExW(WS_EX_TOPMOST | WS_EX_DLGMODALFRAME, CLS_IN, title, WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
                             (wa.left + wa.right - (r.right - r.left)) / 2, (wa.top + wa.bottom - (r.bottom - r.top)) / 2 - Sc(60),
                             r.right - r.left, r.bottom - r.top, parent, NULL, g_inst, &c);
    if (!h) return FALSE;
    if (parent) EnableWindow(parent, FALSE);
    SetForegroundWindow(h);
    MSG msg;
    while (!c.done && GetMessageW(&msg, NULL, 0, 0) > 0)
        if (!IsDialogMessageW(h, &msg)) { TranslateMessage(&msg); DispatchMessageW(&msg); }
    if (parent) { EnableWindow(parent, TRUE); SetForegroundWindow(parent); }
    return c.ok;
}

/* ======================= modern popup menu ======================= */
#define CM_SEP 1
#define CM_CHECK 2
#define CM_DIS 4
#define CM_RED 8
#define CM_HDR 16
typedef struct { int id; WCHAR text[72]; int fl; WCHAR hint[28]; } CmItem;
#define SHM 16
typedef struct {
    Canvas cv;
    CmItem it[28];
    int n, hover, result, done, px, py;
    float y[29], mw, mh, itemH, sepH;
} MenuCtx;

static int CmAdd(CmItem *a, int n, int id, const WCHAR *t, int fl) {
    a[n].id = id;
    lstrcpynW(a[n].text, t ? t : L"", 72);
    a[n].fl = fl;
    a[n].hint[0] = 0;
    return n + 1;
}

static int CmAddH(CmItem *a, int n, int id, const WCHAR *t, int fl, const WCHAR *hint) {
    n = CmAdd(a, n, id, t, fl);
    lstrcpynW(a[n - 1].hint, hint, 28);
    return n;
}

static void MenuDraw(MenuCtx *c, HWND h) {
    GpGraphics *g = c->cv.g;
    GdipGraphicsClear(g, 0);
    float ox = SHM, oy = SHM, pad = (float)Sc(6);
    for (int i = 8; i >= 1; i--) FillRR(g, ARGBC(5, 0, 0, 0), ox - i, oy - i + 4, c->mw + 2 * i, c->mh + 2 * i, 16.f + i);
    FillRR(g, g_dark ? ARGBC(244, 36, 37, 43) : ARGBC(244, 250, 250, 253), ox, oy, c->mw, c->mh, 16);
    StrokeRR(g, g_dark ? ARGBC(70, 255, 255, 255) : ARGBC(45, 0, 0, 0), 1, ox, oy, c->mw, c->mh, 16);
    for (int i = 0; i < c->n; i++) {
        float y = oy + c->y[i];
        CmItem *t = &c->it[i];
        if (t->fl & CM_SEP) {
            GpPen *p;
            GdipCreatePen1(g_dark ? ARGBC(40, 255, 255, 255) : ARGBC(30, 0, 0, 0), 1, 2, &p);
            GdipDrawLine(g, p, ox + pad + 6, y + c->sepH / 2, ox + c->mw - pad - 6, y + c->sepH / 2);
            GdipDeletePen(p);
            continue;
        }
        if (t->fl & CM_HDR) {
            DrawTextBox(g, t->text, g_font, ox + pad + 14, y + 4, c->mw - 2 * pad - 20, c->itemH * 0.7f, g_dark ? ARGBC(150, 200, 202, 214) : ARGBC(160, 90, 94, 108), TRUE);
            continue;
        }
        BOOL hv = (i == c->hover) && !(t->fl & CM_DIS);
        if (hv) FillRR(g, ARGBC(255, 10, 132, 255), ox + pad, y, c->mw - 2 * pad, c->itemH, 9);
        ARGB col = hv ? ARGBC(255, 255, 255, 255) : (t->fl & CM_DIS) ? ARGBC(110, 128, 128, 128)
                 : (t->fl & CM_RED) ? ARGBC(255, 255, 69, 58) : g_dark ? ARGBC(255, 236, 236, 241) : ARGBC(255, 28, 28, 32);
        if (t->fl & CM_CHECK) {
            GpPen *p;
            GdipCreatePen1(col, 1.9f, 2, &p);
            float cx = ox + pad + 12, cy = y + c->itemH / 2;
            GdipDrawLine(g, p, cx, cy, cx + 3.5f, cy + 4);
            GdipDrawLine(g, p, cx + 3.5f, cy + 4, cx + 10, cy - 4);
            GdipDeletePen(p);
        }
        DrawTextBox(g, t->text, g_font, ox + pad + 30, y, c->mw - 2 * pad - 36, c->itemH, col, TRUE);
        if (t->hint[0]) DrawTextBoxR(g, t->hint, g_font, ox + pad + 30, y, c->mw - 2 * pad - 44, c->itemH, hv ? ARGBC(200, 255, 255, 255) : ARGBC(130, 150, 152, 165));
    }
    CanvasPresent(&c->cv, h, c->px, c->py);
}

static int MenuHit(MenuCtx *c, int x, int y) {
    float lx = (float)x - SHM, ly = (float)y - SHM;
    if (lx < 0 || lx > c->mw || ly < 0 || ly > c->mh) return -1;
    for (int i = 0; i < c->n; i++)
        if (!(c->it[i].fl & (CM_SEP | CM_HDR)) && ly >= c->y[i] && ly < c->y[i + 1]) return (c->it[i].fl & CM_DIS) ? -1 : i;
    return -1;
}

static LRESULT CALLBACK MenuProc(HWND h, UINT m, WPARAM w, LPARAM l) {
    MenuCtx *c = (MenuCtx *)GetWindowLongPtrW(h, GWLP_USERDATA);
    if (!c) return DefWindowProcW(h, m, w, l);
    int x = GET_X_LPARAM(l), y = GET_Y_LPARAM(l);
    switch (m) {
    case WM_MOUSEACTIVATE: return MA_NOACTIVATE;
    case WM_MOUSEMOVE: {
        int hv = MenuHit(c, x, y);
        if (hv != c->hover) { c->hover = hv; MenuDraw(c, h); }
        return 0;
    }
    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_MBUTTONDOWN: {
        float lx = (float)x - SHM, ly = (float)y - SHM;
        if (lx < 0 || lx > c->mw || ly < 0 || ly > c->mh) c->done = 1;
        return 0;
    }
    case WM_LBUTTONUP:
    case WM_RBUTTONUP: {
        int hv = MenuHit(c, x, y);
        if (hv >= 0) { c->result = c->it[hv].id; c->done = 1; }
        return 0;
    }
    case WM_CAPTURECHANGED: c->done = 1; return 0;
    case WM_TIMER:
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) c->done = 1;
        return 0;
    }
    return DefWindowProcW(h, m, w, l);
}

static int ShowMenu(const CmItem *items_, int n, int ax, int ay) {
    MenuCtx *c = (MenuCtx *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(MenuCtx));
    if (!c) return 0;
    memcpy(c->it, items_, sizeof(CmItem) * n);
    c->n = n; c->hover = -1;
    c->itemH = (float)Sc(34); c->sepH = (float)Sc(11);
    float pad = (float)Sc(6), y = pad, mx = 0;
    for (int i = 0; i < n; i++) {
        c->y[i] = y;
        if (c->it[i].fl & CM_SEP) y += c->sepH;
        else if (c->it[i].fl & CM_HDR) y += c->itemH * 0.8f;
        else {
            y += c->itemH;
            float tw = TextWidth(g_g, c->it[i].text, g_font) + (c->it[i].hint[0] ? TextWidth(g_g, c->it[i].hint, g_font) + 24 : 0);
            if (tw > mx) mx = tw;
        }
    }
    c->y[n] = y;
    c->mh = y + pad;
    c->mw = mx + 30 + 36 + 2 * pad;
    if (c->mw < Sc(190)) c->mw = (float)Sc(190);
    int W = (int)c->mw + 2 * SHM, H = (int)c->mh + 2 * SHM;
    RECT mr = g_mi.rcMonitor;
    int px, py;
    switch (g_menuEdge >= 0 ? g_menuEdge : cfg.edge) {
    case E_TOP: px = ax - W / 2; py = ay + 8; break;
    case E_LEFT: px = ax + 8; py = ay - H / 2; break;
    case E_RIGHT: px = ax - W - 8; py = ay - H / 2; break;
    default: px = ax - W / 2; py = ay - H - 4; break;
    }
    if (px + W > mr.right) px = mr.right - W;
    if (py + H > mr.bottom) py = mr.bottom - H;
    if (px < mr.left) px = mr.left;
    if (py < mr.top) py = mr.top;
    c->px = px; c->py = py;
    int res = 0;
    if (CanvasNew(&c->cv, W, H)) {
        HWND h = CreateWindowExW(WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE, CLS_MENU, L"", WS_POPUP, px, py, W, H, NULL, NULL, g_inst, NULL);
        SetWindowLongPtrW(h, GWLP_USERDATA, (LONG_PTR)c);
        MenuDraw(c, h);
        ShowWindow(h, SW_SHOWNA);
        SetCapture(h);
        SetTimer(h, 1, 50, NULL);
        MSG msg;
        while (!c->done && GetMessageW(&msg, NULL, 0, 0) > 0) { TranslateMessage(&msg); DispatchMessageW(&msg); }
        SetWindowLongPtrW(h, GWLP_USERDATA, 0);
        if (GetCapture() == h) ReleaseCapture();
        DestroyWindow(h);
        res = c->result;
        CanvasFree(&c->cv);
    }
    HeapFree(GetProcessHeap(), 0, c);
    return res;
}

/* ======================= group popup ======================= */
typedef struct {
    Canvas cv;
    Item *g;
    int gi, cols, rows, hover, done, inMenu, px, py;
    float cw, ch, pad, titleH, W, H;
} GrpCtx;

static void GrpDraw(GrpCtx *c, HWND h) {
    GpGraphics *g = c->cv.g;
    GdipGraphicsClear(g, 0);
    float ox = SHM, oy = SHM;
    for (int i = 8; i >= 1; i--) FillRR(g, ARGBC(5, 0, 0, 0), ox - i, oy - i + 5, c->W + 2 * i, c->H + 2 * i, 22.f + i);
    FillRR(g, g_dark ? ARGBC(240, 40, 41, 48) : ARGBC(240, 246, 247, 251), ox, oy, c->W, c->H, 22);
    StrokeRR(g, g_dark ? ARGBC(60, 255, 255, 255) : ARGBC(40, 0, 0, 0), 1, ox, oy, c->W, c->H, 22);
    ARGB tc = g_dark ? ARGBC(255, 240, 240, 245) : ARGBC(255, 30, 30, 34);
    DrawCentered(g, c->g->name, g_fontBold, ox, oy + 6, c->W, c->titleH, tc);
    DrawTextBoxR(g, L"点击标题重命名", g_font, ox, oy + 8, c->W - 16, c->titleH - 6, g_dark ? ARGBC(110, 200, 202, 214) : ARGBC(130, 90, 94, 108));
    for (int k = 0; k < c->g->nk; k++) {
        int col = k % c->cols, row = k / c->cols;
        float x = ox + c->pad + col * c->cw, y = oy + c->titleH + c->pad + row * c->ch;
        if (k == c->hover) FillRR(g, g_dark ? ARGBC(50, 255, 255, 255) : ARGBC(30, 0, 0, 0), x, y, c->cw, c->ch, 12);
        float is = (float)Sc(54);
        if (c->g->kids[k].icon) DrawImg(g, c->g->kids[k].icon, x + (c->cw - is) / 2, y + 8, is, is, FALSE);
        DrawCentered(g, c->g->kids[k].name, g_font, x + 2, y + 8 + is + 4, c->cw - 4, (float)Sc(22), tc);
    }
    CanvasPresent(&c->cv, h, c->px, c->py);
}

static int GrpHit(GrpCtx *c, int x, int y) {
    float lx = x - SHM - c->pad, ly = y - SHM - c->titleH - c->pad;
    if (lx < 0 || ly < 0) return -1;
    int col = (int)(lx / c->cw), row = (int)(ly / c->ch);
    if (col >= c->cols) return -1;
    int k = row * c->cols + col;
    return (k >= 0 && k < c->g->nk) ? k : -1;
}

static int g_menuItem = -1;
static void ShowRightMenuForKid(GrpCtx *c, int k);

static LRESULT CALLBACK GrpProc(HWND h, UINT m, WPARAM w, LPARAM l) {
    GrpCtx *c = (GrpCtx *)GetWindowLongPtrW(h, GWLP_USERDATA);
    if (!c) return DefWindowProcW(h, m, w, l);
    int x = GET_X_LPARAM(l), y = GET_Y_LPARAM(l);
    switch (m) {
    case WM_MOUSEACTIVATE: return MA_NOACTIVATE;
    case WM_MOUSEMOVE: {
        int hv = GrpHit(c, x, y);
        if (hv != c->hover) { c->hover = hv; GrpDraw(c, h); }
        return 0;
    }
    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN: {
        float lx = (float)x - SHM, ly = (float)y - SHM;
        if (lx < 0 || ly < 0 || lx > c->W || ly > c->H) c->done = 1;
        return 0;
    }
    case WM_LBUTTONUP: {
        int k = GrpHit(c, x, y);
        if (k >= 0) { LaunchPtr(&c->g->kids[k]); c->done = 1; }
        else if (y > SHM && y < SHM + c->titleH + 4 && x > SHM && x < SHM + c->W) {
            WCHAR nm[128];
            lstrcpynW(nm, c->g->name, 128);
            c->inMenu = 1;
            ReleaseCapture();
            BOOL ok = InputBox(g_hset, L"应用组名称", L"名称：", nm, 128, NULL, NULL, 0);
            c->inMenu = 0;
            SetCapture(h);
            if (ok && nm[0]) { lstrcpynW(c->g->name, nm, 128); CleanName(c->g->name); ScheduleSave(); GrpDraw(c, h); }
        }
        return 0;
    }
    case WM_RBUTTONUP: {
        int k = GrpHit(c, x, y);
        if (k >= 0) ShowRightMenuForKid(c, k);
        return 0;
    }
    case WM_CAPTURECHANGED: if (!c->inMenu) c->done = 1; return 0;
    case WM_TIMER: if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) c->done = 1; return 0;
    }
    return DefWindowProcW(h, m, w, l);
}

static HWND g_grpWnd;
static void ShowRightMenuForKid(GrpCtx *c, int k) {
    CmItem it[6];
    int n = 0;
    n = CmAdd(it, n, 1, L"打开", 0);
    n = CmAdd(it, n, 2, L"移出到 Dock", 0);
    n = CmAdd(it, n, 3, L"从组中删除", CM_RED);
    POINT p;
    GetCursorPos(&p);
    c->inMenu = 1;
    int r = ShowMenu(it, n, p.x, p.y);
    c->inMenu = 0;
    SetCapture(g_grpWnd);
    if (r == 1) { LaunchPtr(&c->g->kids[k]); c->done = 1; }
    else if (r == 2) { ExtractKid(c->gi, k); c->done = 1; }
    else if (r == 3) {
        if (c->g->kids[k].icon) GdipDisposeImage(c->g->kids[k].icon);
        memmove(&c->g->kids[k], &c->g->kids[k + 1], sizeof(Item) * (c->g->nk - k - 1));
        c->g->nk--;
        BuildGroupIcon(c->g);
        ScheduleSave();
        c->done = 1;
    }
    Kick();
}

static void ShowGroup(int idx) {
    if (idx < 0 || idx >= nItems || items[idx].type != T_GROUP) return;
    Item *g = &items[idx];
    if (g->nk == 0) { MessageBoxW(g_hset, L"这个应用组还是空的。\n把应用图标拖到组图标上即可加入。", L"MiniDock", MB_OK | MB_ICONINFORMATION); return; }
    GrpCtx *c = (GrpCtx *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(GrpCtx));
    if (!c) return;
    c->g = g; c->gi = idx; c->hover = -1;
    c->cols = g->nk < 4 ? g->nk : 4;
    c->rows = (g->nk + c->cols - 1) / c->cols;
    c->cw = (float)Sc(92); c->ch = (float)Sc(94); c->pad = (float)Sc(16); c->titleH = (float)Sc(34);
    c->W = c->cols * c->cw + 2 * c->pad;
    if (c->W < Sc(200)) c->W = (float)Sc(200);
    c->H = c->titleH + c->rows * c->ch + 2 * c->pad;
    int W = (int)c->W + 2 * SHM, H = (int)c->H + 2 * SHM;
    RECT ir;
    IconScreenRect(idx, &ir);
    int px, py;
    switch (cfg.edge) {
    case E_TOP: px = (ir.left + ir.right) / 2 - W / 2; py = ir.bottom + 6; break;
    case E_LEFT: px = ir.right + 6; py = (ir.top + ir.bottom) / 2 - H / 2; break;
    case E_RIGHT: px = ir.left - W - 6; py = (ir.top + ir.bottom) / 2 - H / 2; break;
    default: px = (ir.left + ir.right) / 2 - W / 2; py = ir.top - H - 6; break;
    }
    RECT mr = g_mi.rcMonitor;
    if (px + W > mr.right) px = mr.right - W;
    if (py + H > mr.bottom) py = mr.bottom - H;
    if (px < mr.left) px = mr.left;
    if (py < mr.top) py = mr.top;
    c->px = px; c->py = py;
    if (CanvasNew(&c->cv, W, H)) {
        HWND h = CreateWindowExW(WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE, CLS_GRP, L"", WS_POPUP, px, py, W, H, NULL, NULL, g_inst, NULL);
        g_grpWnd = h;
        SetWindowLongPtrW(h, GWLP_USERDATA, (LONG_PTR)c);
        GrpDraw(c, h);
        ShowWindow(h, SW_SHOWNA);
        gMenuOpen = TRUE;
        SetCapture(h);
        SetTimer(h, 1, 50, NULL);
        MSG msg;
        while (!c->done && GetMessageW(&msg, NULL, 0, 0) > 0) { TranslateMessage(&msg); DispatchMessageW(&msg); }
        gMenuOpen = FALSE;
        SetWindowLongPtrW(h, GWLP_USERDATA, 0);
        if (GetCapture() == h) ReleaseCapture();
        DestroyWindow(h);
        g_grpWnd = NULL;
        CanvasFree(&c->cv);
    }
    HeapFree(GetProcessHeap(), 0, c);
    Kick();
}

/* ======================= hover preview (DWM thumbnails) ======================= */
static HWND pvWin[6];
static HTHUMBNAIL pvTh[6];
static WCHAR pvTitle[6][96];
static RECT pvCell[6];
static int pvN, pvHot = -1;

typedef struct { const WCHAR *exe; int n; } PvCtx;
static BOOL CALLBACK PvEnum(HWND h, LPARAM lp) {
    PvCtx *c = (PvCtx *)lp;
    if (c->n >= 6 || !IsAppWindow(h) || h == g_hwnd) return TRUE;
    WCHAR exe[MAX_PATH];
    if (!ExeOfWindow(h, exe) || lstrcmpiW(exe, c->exe) != 0) return TRUE;
    pvWin[c->n] = h;
    GetWindowTextW(h, pvTitle[c->n], 96);
    c->n++;
    return TRUE;
}

void ClosePreview(void) {
    for (int i = 0; i < 6; i++) if (pvTh[i]) { DwmUnregisterThumbnail(pvTh[i]); pvTh[i] = NULL; }
    if (g_pv) { HWND h = g_pv; g_pv = NULL; DestroyWindow(h); }
    pvItem = -1; pvN = 0; pvHot = -1;
    Kick();
}

static LRESULT CALLBACK PvProc(HWND h, UINT m, WPARAM w, LPARAM l) {
    switch (m) {
    case WM_MOUSEACTIVATE: return MA_NOACTIVATE;
    case WM_ERASEBKGND: return 1;
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC dc = BeginPaint(h, &ps);
        RECT rc;
        GetClientRect(h, &rc);
        HBRUSH bg = CreateSolidBrush(g_dark ? RGB(36, 37, 43) : RGB(246, 247, 251));
        FillRect(dc, &rc, bg);
        DeleteObject(bg);
        SetBkMode(dc, TRANSPARENT);
        SelectObject(dc, g_uiFont);
        SetTextColor(dc, g_dark ? RGB(238, 238, 243) : RGB(30, 30, 34));
        for (int i = 0; i < pvN; i++) {
            if (i == pvHot) {
                HBRUSH hb = CreateSolidBrush(g_dark ? RGB(62, 64, 74) : RGB(222, 226, 238));
                HPEN pn = (HPEN)GetStockObject(NULL_PEN);
                SelectObject(dc, hb); SelectObject(dc, pn);
                RoundRect(dc, pvCell[i].left - Sc(4), pvCell[i].top - Sc(4), pvCell[i].right + Sc(4), pvCell[i].bottom + Sc(4), Sc(12), Sc(12));
                DeleteObject(hb);
            }
            RECT tr = { pvCell[i].left, pvCell[i].bottom - Sc(22), pvCell[i].right, pvCell[i].bottom };
            DrawTextW(dc, pvTitle[i], -1, &tr, DT_SINGLELINE | DT_END_ELLIPSIS | DT_CENTER | DT_VCENTER);
        }
        EndPaint(h, &ps);
        return 0;
    }
    case WM_MOUSEMOVE: {
        POINT p = { GET_X_LPARAM(l), GET_Y_LPARAM(l) };
        int hv = -1;
        for (int i = 0; i < pvN; i++) if (PtInRect(&pvCell[i], p)) hv = i;
        if (hv != pvHot) { pvHot = hv; InvalidateRect(h, NULL, TRUE); }
        return 0;
    }
    case WM_LBUTTONUP: {
        POINT p = { GET_X_LPARAM(l), GET_Y_LPARAM(l) };
        for (int i = 0; i < pvN; i++)
            if (PtInRect(&pvCell[i], p)) {
                HWND t = pvWin[i];
                if (IsIconic(t)) ShowWindow(t, SW_RESTORE);
                SetForegroundWindow(t);
                break;
            }
        ClosePreview();
        return 0;
    }
    case WM_MBUTTONUP: {
        POINT p = { GET_X_LPARAM(l), GET_Y_LPARAM(l) };
        for (int i = 0; i < pvN; i++) if (PtInRect(&pvCell[i], p)) PostMessageW(pvWin[i], WM_CLOSE, 0, 0);
        ClosePreview();
        return 0;
    }
    }
    return DefWindowProcW(h, m, w, l);
}

static void ShowPreview(int idx) {
    if (g_pv || idx < 0 || idx >= nItems || !items[idx].target[0]) return;
    PvCtx c = { items[idx].target, 0 };
    EnumWindows(PvEnum, (LPARAM)&c);
    if (c.n == 0) return;
    pvN = c.n;
    int tw = Sc(200), th = Sc(125), lab = Sc(24), gap = Sc(12), pad = Sc(12);
    int W = pad * 2 + pvN * tw + (pvN - 1) * gap + Sc(8), H = pad * 2 + th + lab + Sc(8);
    RECT ir;
    IconScreenRect(idx, &ir);
    int px, py;
    switch (cfg.edge) {
    case E_TOP: px = (ir.left + ir.right) / 2 - W / 2; py = ir.bottom + 10; break;
    case E_LEFT: px = ir.right + 10; py = (ir.top + ir.bottom) / 2 - H / 2; break;
    case E_RIGHT: px = ir.left - W - 10; py = (ir.top + ir.bottom) / 2 - H / 2; break;
    default: px = (ir.left + ir.right) / 2 - W / 2; py = ir.top - H - 10; break;
    }
    RECT mr = g_mi.rcMonitor;
    if (px + W > mr.right) px = mr.right - W;
    if (py + H > mr.bottom) py = mr.bottom - H;
    if (px < mr.left) px = mr.left;
    if (py < mr.top) py = mr.top;
    HWND h = CreateWindowExW(WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE, CLS_PV, L"", WS_POPUP, px, py, W, H, NULL, NULL, g_inst, NULL);
    if (!h) { pvN = 0; return; }
    g_pv = h;
    SetWindowRgn(h, CreateRoundRectRgn(0, 0, W + 1, H + 1, Sc(22), Sc(22)), TRUE);
    for (int i = 0; i < pvN; i++) {
        int x = pad + Sc(4) + i * (tw + gap), y = pad + Sc(4);
        pvCell[i].left = x; pvCell[i].top = y; pvCell[i].right = x + tw; pvCell[i].bottom = y + th + lab;
        pvTh[i] = NULL;
        if (SUCCEEDED(DwmRegisterThumbnail(h, pvWin[i], &pvTh[i]))) {
            SIZE ss = { 0, 0 };
            DwmQueryThumbnailSourceSize(pvTh[i], &ss);
            int dw = tw, dh = th;
            if (ss.cx > 0 && ss.cy > 0) {
                float sc = (float)tw / ss.cx;
                if (ss.cy * sc > th) sc = (float)th / ss.cy;
                dw = (int)(ss.cx * sc); dh = (int)(ss.cy * sc);
            }
            DWM_THUMBNAIL_PROPERTIES tp;
            ZeroMemory(&tp, sizeof tp);
            tp.dwFlags = DWM_TNP_RECTDESTINATION | DWM_TNP_VISIBLE | DWM_TNP_OPACITY;
            tp.rcDestination.left = x + (tw - dw) / 2; tp.rcDestination.top = y + (th - dh) / 2;
            tp.rcDestination.right = tp.rcDestination.left + dw; tp.rcDestination.bottom = tp.rcDestination.top + dh;
            tp.fVisible = TRUE; tp.opacity = 255;
            DwmUpdateThumbnailProperties(pvTh[i], &tp);
        }
    }
    pvItem = idx;
    ShowWindow(h, SW_SHOWNA);
    Kick();
}

/* ======================= OLE drop target ======================= */
static UINT cfUrlW, cfUrlA, cfShellIDList;
static BOOL g_dropOK, g_dropFiles;

static BOOL HasFmt(IDataObject *d, UINT cf) {
    FORMATETC fe = { (CLIPFORMAT)cf, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
    return IDataObject_QueryGetData(d, &fe) == S_OK;
}
static BOOL AcceptData(IDataObject *d) {
    return HasFmt(d, CF_HDROP) || HasFmt(d, cfShellIDList) || HasFmt(d, cfUrlW) || HasFmt(d, cfUrlA) || HasFmt(d, CF_UNICODETEXT);
}

static int CenterHit(float u) {
    for (int i = 0; i < nItems; i++) {
        Item *it = &items[i];
        if ((it->type == T_FILE && PathMatchSpecW(it->path, L"*.exe;*.lnk")) || it->type == T_GROUP)
            if (it->cur > M.S * 0.5f && fabsf(u - actC[i]) < it->cur * 0.3f) return i;
    }
    return -1;
}

static void DropPoint(POINTL pt, BOOL files) {
    POINT p = { pt.x, pt.y };
    ScreenToClient(g_hwnd, &p);
    float u, v;
    ToLogical((float)p.x, (float)p.y, &u, &v);
    int on = files ? CenterHit(u) : -1;
    if (on >= 0) { mergeT = on; gapIdx = -1; return; }
    mergeT = -1;
    float G = (gapCur > 0.5f) ? gapCur + M.g : 0;
    int idx = 0, pc = PinnedCount();
    for (int i = 0; i < nItems; i++) {
        float c = actC[i];
        if (G > 0) c += (gapIdx >= 0 && i >= gapIdx) ? -G / 2 : G / 2;
        if (c < u) idx = i + 1;
    }
    if (idx > pc) idx = pc;
    gapIdx = idx;
}

static HRESULT STDMETHODCALLTYPE DT_QI(IDropTarget *t, REFIID riid, void **ppv) {
    if (IsEqualIID(riid, &IID_IUnknown) || IsEqualIID(riid, &IID_IDropTarget)) { *ppv = t; return S_OK; }
    *ppv = NULL;
    return E_NOINTERFACE;
}
static ULONG STDMETHODCALLTYPE DT_AddRef(IDropTarget *t) { return 2; }
static ULONG STDMETHODCALLTYPE DT_Release(IDropTarget *t) { return 1; }
static HRESULT STDMETHODCALLTYPE DT_Enter(IDropTarget *t, IDataObject *d, DWORD k, POINTL pt, DWORD *eff) {
    g_dropOK = AcceptData(d);
    g_dropFiles = HasFmt(d, CF_HDROP);
    if (g_dropOK) { DropPoint(pt, g_dropFiles); *eff = DROPEFFECT_LINK; Kick(); } else *eff = DROPEFFECT_NONE;
    return S_OK;
}
static HRESULT STDMETHODCALLTYPE DT_Over(IDropTarget *t, DWORD k, POINTL pt, DWORD *eff) {
    if (g_dropOK) { DropPoint(pt, g_dropFiles); *eff = DROPEFFECT_LINK; Kick(); } else *eff = DROPEFFECT_NONE;
    return S_OK;
}
static HRESULT STDMETHODCALLTYPE DT_Leave(IDropTarget *t) { gapIdx = -1; mergeT = -1; Kick(); return S_OK; }

static BOOL GetGlobalText(IDataObject *d, UINT cf, BOOL wide, WCHAR *out, int cch) {
    FORMATETC fe = { (CLIPFORMAT)cf, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
    STGMEDIUM sm;
    if (IDataObject_GetData(d, &fe, &sm) != S_OK) return FALSE;
    BOOL ok = FALSE;
    void *p = GlobalLock(sm.hGlobal);
    if (p) {
        if (wide) lstrcpynW(out, (WCHAR *)p, cch); else MultiByteToWideChar(CP_ACP, 0, (char *)p, -1, out, cch);
        GlobalUnlock(sm.hGlobal);
        ok = out[0] != 0;
    }
    ReleaseStgMedium(&sm);
    return ok;
}

static HRESULT STDMETHODCALLTYPE DT_Drop(IDropTarget *t, IDataObject *d, DWORD k, POINTL pt, DWORD *eff) {
    int idx = gapIdx >= 0 ? gapIdx : PinnedCount();
    int on = mergeT;
    float startCur = gapCur;
    int first = idx;
    BOOL any = FALSE;
    FORMATETC fe = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
    STGMEDIUM sm;
    if (IDataObject_GetData(d, &fe, &sm) == S_OK) {
        HDROP hd = (HDROP)GlobalLock(sm.hGlobal);
        if (hd) {
            UINT cnt = DragQueryFileW(hd, 0xFFFFFFFF, NULL, 0);
            if (on >= 0 && on < nItems && items[on].type == T_FILE) { /* drop files onto an app icon: open them with it */
                WCHAR params[3000] = L"";
                for (UINT i = 0; i < cnt; i++) {
                    WCHAR f[1024];
                    DragQueryFileW(hd, i, f, 1024);
                    if (lstrlenW(params) + lstrlenW(f) + 4 < 2990) { lstrcatW(params, L"\""); lstrcatW(params, f); lstrcatW(params, L"\" "); }
                }
                WCHAR dir[MAX_PATH];
                lstrcpynW(dir, items[on].path, MAX_PATH);
                PathRemoveFileSpecW(dir);
                ShellExecuteW(NULL, L"open", items[on].target[0] ? items[on].target : items[on].path, params, dir, SW_SHOWNORMAL);
                if (cfg.bounce) items[on].bounce = GetTickCount() | 1;
                any = TRUE; on = -2;
            } else if (on >= 0 && on < nItems && items[on].type == T_GROUP) {
                for (UINT i = 0; i < cnt; i++) {
                    WCHAR f[1024];
                    DragQueryFileW(hd, i, f, 1024);
                    Item kid;
                    if (BuildItem(&kid, f)) { if (!KidAdd(&items[on], &kid)) FreeItem(&kid); }
                }
                ScheduleSave();
                any = TRUE; on = -2;
            } else {
                for (UINT i = 0; i < cnt; i++) {
                    WCHAR f[1024];
                    DragQueryFileW(hd, i, f, 1024);
                    int r = AddFromString(f, idx);
                    if (r >= 0) { idx = r + 1; any = TRUE; }
                }
            }
            GlobalUnlock(sm.hGlobal);
        }
        ReleaseStgMedium(&sm);
    } else {
        WCHAR s[1024];
        if (GetGlobalText(d, cfUrlW, TRUE, s, 1024) || GetGlobalText(d, cfUrlA, FALSE, s, 1024) || GetGlobalText(d, CF_UNICODETEXT, TRUE, s, 1024)) {
            WCHAR *nl = wcspbrk(s, L"\r\n");
            if (nl) *nl = 0;
            int r = AddFromString(s, idx);
            if (r >= 0) { idx = r + 1; any = TRUE; }
        }
    }
    if (!any && on != -2 && HasFmt(d, cfShellIDList)) {   /* Start-menu items (apps, Edge/Chrome web apps) */
        FORMATETC fe2 = { (CLIPFORMAT)cfShellIDList, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
        STGMEDIUM sm2;
        if (IDataObject_GetData(d, &fe2, &sm2) == S_OK) {
            CIDA *ci = (CIDA *)GlobalLock(sm2.hGlobal);
            if (ci) {
                LPCITEMIDLIST parent = (LPCITEMIDLIST)((BYTE *)ci + ci->aoffset[0]);
                for (UINT i = 0; i < ci->cidl; i++) {
                    LPCITEMIDLIST child = (LPCITEMIDLIST)((BYTE *)ci + ci->aoffset[i + 1]);
                    LPITEMIDLIST full = ILCombine(parent, child);
                    if (full) {
                        Item it;
                        if (BuildFromPidl(&it, full)) {
                            int r = InsertItemStruct(idx, &it, FALSE);
                            if (r >= 0) { idx = r + 1; any = TRUE; ScheduleSave(); }
                        }
                        ILFree(full);
                    }
                }
                GlobalUnlock(sm2.hGlobal);
            }
            ReleaseStgMedium(&sm2);
        }
    }
    gapIdx = -1; mergeT = -1; gapCur = 0;
    if (any) {
        if (on != -2) for (int i = first; i < idx && i < nItems; i++) items[i].cur = startCur / (idx - first);
        *eff = DROPEFFECT_LINK;
        UpdateGeometry();
    } else { *eff = DROPEFFECT_NONE; PostMessageW(g_hwnd, WM_APP_FAIL, 0, 0); }
    Kick();
    return S_OK;
}

static IDropTargetVtbl g_dtVtbl = { DT_QI, DT_AddRef, DT_Release, DT_Enter, DT_Over, DT_Leave, DT_Drop };
static IDropTarget g_dt = { &g_dtVtbl };

/* ======================= folder "fan" (mac style stack) ======================= */
typedef struct { WCHAR name[96]; WCHAR path[MAX_PATH]; FILETIME ft; BOOL isDir; } FanLite;
typedef struct {
    Canvas cv;
    int n, hover, done, px, py;
    WCHAR name[13][96], path[13][MAX_PATH];
    GpBitmap *icon[13];
    float cx[13], cy[13], isz, lw[13], lx[13], ly[13];
    BOOL lab[13];
} FanCtx;

static int FanCmp(const void *a, const void *b) {
    return CompareFileTime(&((const FanLite *)b)->ft, &((const FanLite *)a)->ft);
}

static void FanDraw(FanCtx *c, HWND h) {
    GpGraphics *g = c->cv.g;
    GdipGraphicsClear(g, 0);
    for (int k = c->n - 1; k >= 0; k--) {
        float sz = c->isz * (k == c->hover ? 1.14f : 1.f);
        if (c->icon[k]) DrawImg(g, c->icon[k], c->cx[k] - sz / 2, c->cy[k] - sz / 2, sz, sz, FALSE);
        FillRR(g, k == c->hover ? ARGBC(255, 10, 132, 255) : ARGBC(232, 36, 37, 42), c->lx[k], c->ly[k], c->lw[k], 28, 10);
        DrawCentered(g, c->name[k], g_font, c->lx[k], c->ly[k], c->lw[k], 28, ARGBC(255, 246, 246, 248));
    }
    CanvasPresent(&c->cv, h, c->px, c->py);
}

static int FanHit(FanCtx *c, int x, int y) {
    for (int k = 0; k < c->n; k++) {
        float hs = c->isz / 2;
        if (x >= c->cx[k] - hs && x <= c->cx[k] + hs && y >= c->cy[k] - hs && y <= c->cy[k] + hs) return k;
        if (x >= c->lx[k] && x <= c->lx[k] + c->lw[k] && y >= c->ly[k] && y <= c->ly[k] + 28) return k;
    }
    return -1;
}

static LRESULT CALLBACK FanProc(HWND h, UINT m, WPARAM w, LPARAM l) {
    FanCtx *c = (FanCtx *)GetWindowLongPtrW(h, GWLP_USERDATA);
    if (!c) return DefWindowProcW(h, m, w, l);
    int x = GET_X_LPARAM(l), y = GET_Y_LPARAM(l);
    switch (m) {
    case WM_MOUSEACTIVATE: return MA_NOACTIVATE;
    case WM_MOUSEMOVE: { int hv = FanHit(c, x, y); if (hv != c->hover) { c->hover = hv; FanDraw(c, h); } return 0; }
    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN: if (FanHit(c, x, y) < 0) c->done = 1; return 0;
    case WM_LBUTTONUP: {
        int k = FanHit(c, x, y);
        if (k >= 0) { ShellExecuteW(NULL, L"open", c->path[k], NULL, NULL, SW_SHOWNORMAL); c->done = 1; }
        return 0;
    }
    case WM_CAPTURECHANGED: c->done = 1; return 0;
    case WM_TIMER: if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) c->done = 1; return 0;
    }
    return DefWindowProcW(h, m, w, l);
}

static void ShowFan(int idx) {
    if (idx < 0 || idx >= nItems) return;
    Item *it = &items[idx];
    WCHAR pat[MAX_PATH + 4];
    PathCombineW(pat, it->path, L"*");
    FanLite *list = (FanLite *)HeapAlloc(GetProcessHeap(), 0, sizeof(FanLite) * 300);
    if (!list) return;
    int cnt = 0;
    WIN32_FIND_DATAW fd;
    HANDLE ff = FindFirstFileW(pat, &fd);
    if (ff != INVALID_HANDLE_VALUE) {
        do {
            if (fd.cFileName[0] == L'.' || (fd.dwFileAttributes & (FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM)) || cnt >= 300) continue;
            lstrcpynW(list[cnt].name, fd.cFileName, 96);
            PathCombineW(list[cnt].path, it->path, fd.cFileName);
            list[cnt].ft = fd.ftLastWriteTime;
            list[cnt].isDir = (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
            cnt++;
        } while (FindNextFileW(ff, &fd));
        FindClose(ff);
    }
    if (cnt == 0) { HeapFree(GetProcessHeap(), 0, list); ShellExecuteW(NULL, L"open", it->path, NULL, NULL, SW_SHOWNORMAL); return; }
    qsort(list, cnt, sizeof(FanLite), FanCmp);
    FanCtx *c = (FanCtx *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(FanCtx));
    if (!c) { HeapFree(GetProcessHeap(), 0, list); return; }
    int n = cnt < 10 ? cnt : 10;
    c->hover = -1;
    c->isz = (float)Sc(46);
    for (int k = 0; k < n; k++) {
        lstrcpynW(c->name[k], list[k].name, 96);
        lstrcpynW(c->path[k], list[k].path, MAX_PATH);
        c->icon[k] = ShellIconBitmap(list[k].path, NULL, 96);
    }
    lstrcpyW(c->name[n], L"在资源管理器中打开…");
    lstrcpynW(c->path[n], it->path, MAX_PATH);
    c->icon[n] = NULL;
    c->n = n + 1;
    HeapFree(GetProcessHeap(), 0, list);

    RECT ir;
    IconScreenRect(idx, &ir);
    float cx0 = (float)(ir.left + ir.right) / 2, cy0 = (float)(ir.top + ir.bottom) / 2;
    float dx = 0, dy = -1;
    switch (cfg.edge) { case E_TOP: dy = 1; break; case E_LEFT: dx = 1; dy = 0; break; case E_RIGHT: dx = -1; dy = 0; break; default: break; }
    RECT mr = g_mi.rcMonitor;
    float mc = Horiz() ? (mr.left + mr.right) / 2.f : (mr.top + mr.bottom) / 2.f;
    float sign = ((Horiz() ? cx0 : cy0) < mc) ? 1.f : -1.f;
    float minx = 1e9f, miny = 1e9f, maxx = -1e9f, maxy = -1e9f;
    for (int k = 0; k < c->n; k++) {
        float d = c->isz * 0.9f + k * (c->isz + Sc(9)), lat = sign * (k * k * c->isz * 0.07f);
        c->cx[k] = cx0 + dx * d + (Horiz() ? lat : 0);
        c->cy[k] = cy0 + dy * d + (Horiz() ? 0 : lat);
        c->lw[k] = TextWidth(g_g, c->name[k], g_font) + 26;
        if (c->lw[k] > 300) c->lw[k] = 300;
        if (Horiz()) { c->lx[k] = sign > 0 ? c->cx[k] - c->isz / 2 - 12 - c->lw[k] : c->cx[k] + c->isz / 2 + 12; c->ly[k] = c->cy[k] - 14; }
        else { c->lx[k] = c->cx[k] - c->lw[k] / 2; c->ly[k] = c->cy[k] + c->isz / 2 + 6; }
        float l0 = c->cx[k] - c->isz / 2, r0 = c->cx[k] + c->isz / 2, t0 = c->cy[k] - c->isz / 2, b0 = c->cy[k] + c->isz / 2;
        if (c->lx[k] < l0) l0 = c->lx[k];
        if (c->lx[k] + c->lw[k] > r0) r0 = c->lx[k] + c->lw[k];
        if (c->ly[k] < t0) t0 = c->ly[k];
        if (c->ly[k] + 28 > b0) b0 = c->ly[k] + 28;
        if (l0 < minx) minx = l0;
        if (t0 < miny) miny = t0;
        if (r0 > maxx) maxx = r0;
        if (b0 > maxy) maxy = b0;
    }
    float shiftx = 0, shifty = 0;
    if (minx < mr.left + 4) shiftx = mr.left + 4 - minx;
    if (maxx > mr.right - 4) shiftx = mr.right - 4 - maxx;
    if (miny < mr.top + 4) shifty = mr.top + 4 - miny;
    if (maxy > mr.bottom - 4) shifty = mr.bottom - 4 - maxy;
    int W = (int)(maxx - minx) + 8, H = (int)(maxy - miny) + 8;
    c->px = (int)(minx + shiftx) - 4; c->py = (int)(miny + shifty) - 4;
    for (int k = 0; k < c->n; k++) {
        c->cx[k] += shiftx - c->px; c->cy[k] += shifty - c->py;
        c->lx[k] += shiftx - c->px; c->ly[k] += shifty - c->py;
    }
    if (c->n > 0) c->icon[n] = it->icon;   /* folder icon for the "open" entry (not owned) */
    if (CanvasNew(&c->cv, W, H)) {
        HWND h = CreateWindowExW(WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE, CLS_FAN, L"", WS_POPUP, c->px, c->py, W, H, NULL, NULL, g_inst, NULL);
        SetWindowLongPtrW(h, GWLP_USERDATA, (LONG_PTR)c);
        FanDraw(c, h);
        ShowWindow(h, SW_SHOWNA);
        gMenuOpen = TRUE;
        ClosePreview();
        SetCapture(h);
        SetTimer(h, 1, 50, NULL);
        MSG msg;
        while (!c->done && GetMessageW(&msg, NULL, 0, 0) > 0) { TranslateMessage(&msg); DispatchMessageW(&msg); }
        gMenuOpen = FALSE;
        SetWindowLongPtrW(h, GWLP_USERDATA, 0);
        if (GetCapture() == h) ReleaseCapture();
        DestroyWindow(h);
        CanvasFree(&c->cv);
    }
    for (int k = 0; k < n; k++) if (c->icon[k]) GdipDisposeImage(c->icon[k]);
    HeapFree(GetProcessHeap(), 0, c);
    Kick();
}

/* ======================= quick note ("速记") ======================= */
static HWND g_noteWnd, g_noteEdit, g_noteInfo;
static WCHAR g_noteDir[MAX_PATH], g_noteDraft[16384];

static void NoteDirLoad(void) {
    GetPrivateProfileStringW(L"Notes", L"Dir", L"", g_noteDir, MAX_PATH, g_ini);
    if (!g_noteDir[0]) lstrcpyW(g_noteDir, g_dir);
}
static void NoteDirSet(const WCHAR *d) {
    lstrcpynW(g_noteDir, d, MAX_PATH);
    WritePrivateProfileStringW(L"Notes", L"Dir", d, g_ini);
    if (g_noteInfo) { WCHAR t[MAX_PATH + 32]; wsprintfW(t, L"保存位置：%s\\速记.txt", g_noteDir); SetWindowTextW(g_noteInfo, t); }
}

static BOOL NoteAppend(const WCHAR *file, const WCHAR *text) {
    BOOL isNew = GetFileAttributesW(file) == INVALID_FILE_ATTRIBUTES;
    HANDLE h = CreateFileW(file, FILE_APPEND_DATA, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE) return FALSE;
    DWORD w;
    if (isNew) { static const BYTE bom[3] = { 0xEF, 0xBB, 0xBF }; WriteFile(h, bom, 3, &w, NULL); }
    SYSTEMTIME st;
    GetLocalTime(&st);
    int len = lstrlenW(text) + 64;
    WCHAR *all = (WCHAR *)HeapAlloc(GetProcessHeap(), 0, len * sizeof(WCHAR));
    if (!all) { CloseHandle(h); return FALSE; }
    wsprintfW(all, L"[%04d-%02d-%02d %02d:%02d]\r\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute);
    lstrcatW(all, text);
    lstrcatW(all, L"\r\n\r\n");
    int bl = WideCharToMultiByte(CP_UTF8, 0, all, -1, NULL, 0, NULL, NULL);
    char *u8 = (char *)HeapAlloc(GetProcessHeap(), 0, bl);
    if (u8) { WideCharToMultiByte(CP_UTF8, 0, all, -1, u8, bl, NULL, NULL); WriteFile(h, u8, bl - 1, &w, NULL); HeapFree(GetProcessHeap(), 0, u8); }
    HeapFree(GetProcessHeap(), 0, all);
    CloseHandle(h);
    return TRUE;
}

static void NotePickDir(HWND parent, int kind) {
    WCHAR p[MAX_PATH] = L"";
    if (kind == 3) SHGetFolderPathW(NULL, CSIDL_DESKTOPDIRECTORY, NULL, 0, p);
    else if (kind == 4) SHGetFolderPathW(NULL, CSIDL_PERSONAL, NULL, 0, p);
    else if (kind == 5) { PWSTR kp = NULL; if (SUCCEEDED(SHGetKnownFolderPath(&FOLDERID_Downloads, 0, NULL, &kp)) && kp) { lstrcpynW(p, kp, MAX_PATH); CoTaskMemFree(kp); } }
    else {
        BROWSEINFOW b;
        ZeroMemory(&b, sizeof b);
        b.hwndOwner = parent;
        b.lpszTitle = L"选择速记保存的文件夹";
        b.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
        LPITEMIDLIST pi = SHBrowseForFolderW(&b);
        if (pi) { SHGetPathFromIDListW(pi, p); CoTaskMemFree(pi); }
    }
    if (p[0]) NoteDirSet(p);
}

static LRESULT CALLBACK NoteProc(HWND h, UINT m, WPARAM w, LPARAM l) {
    switch (m) {
    case WM_CREATE: {
        int W = Sc(420);
        g_noteEdit = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", g_noteDraft, WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN,
                                     Sc(14), Sc(14), W - Sc(28), Sc(200), h, (HMENU)100, g_inst, NULL);
        CreateWindowW(L"BUTTON", L"保存并清空", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, Sc(14), Sc(224), Sc(110), Sc(32), h, (HMENU)1, g_inst, NULL);
        CreateWindowW(L"BUTTON", L"导出为 txt…", WS_CHILD | WS_VISIBLE, Sc(134), Sc(224), Sc(110), Sc(32), h, (HMENU)2, g_inst, NULL);
        CreateWindowW(L"BUTTON", L"打开文件", WS_CHILD | WS_VISIBLE, Sc(254), Sc(224), Sc(80), Sc(32), h, (HMENU)7, g_inst, NULL);
        g_noteInfo = CreateWindowW(L"STATIC", L"", WS_CHILD | WS_VISIBLE | SS_PATHELLIPSIS, Sc(14), Sc(268), W - Sc(28), Sc(20), h, NULL, g_inst, NULL);
        CreateWindowW(L"STATIC", L"常用位置：", WS_CHILD | WS_VISIBLE, Sc(14), Sc(298), Sc(70), Sc(22), h, NULL, g_inst, NULL);
        CreateWindowW(L"BUTTON", L"桌面", WS_CHILD | WS_VISIBLE, Sc(88), Sc(294), Sc(56), Sc(28), h, (HMENU)3, g_inst, NULL);
        CreateWindowW(L"BUTTON", L"文档", WS_CHILD | WS_VISIBLE, Sc(150), Sc(294), Sc(56), Sc(28), h, (HMENU)4, g_inst, NULL);
        CreateWindowW(L"BUTTON", L"下载", WS_CHILD | WS_VISIBLE, Sc(212), Sc(294), Sc(56), Sc(28), h, (HMENU)5, g_inst, NULL);
        CreateWindowW(L"BUTTON", L"自定义…", WS_CHILD | WS_VISIBLE, Sc(274), Sc(294), Sc(72), Sc(28), h, (HMENU)6, g_inst, NULL);
        SetFontAll(h);
        NoteDirSet(g_noteDir);
        SetFocus(g_noteEdit);
        return 0;
    }
    case WM_COMMAND: {
        int id = LOWORD(w);
        if (id == 1) {
            int n = GetWindowTextLengthW(g_noteEdit);
            if (n > 0) {
                WCHAR *t = (WCHAR *)HeapAlloc(GetProcessHeap(), 0, (n + 2) * sizeof(WCHAR));
                if (t) {
                    GetWindowTextW(g_noteEdit, t, n + 1);
                    WCHAR f[MAX_PATH];
                    PathCombineW(f, g_noteDir, L"速记.txt");
                    if (NoteAppend(f, t)) { SetWindowTextW(g_noteEdit, L""); g_noteDraft[0] = 0; }
                    else MessageBoxW(h, L"无法写入该位置，请换一个保存文件夹。", L"速记", MB_OK | MB_ICONWARNING);
                    HeapFree(GetProcessHeap(), 0, t);
                }
            }
        } else if (id == 2) {
            WCHAR src[MAX_PATH], dst[MAX_PATH] = L"速记.txt";
            PathCombineW(src, g_noteDir, L"速记.txt");
            if (GetFileAttributesW(src) == INVALID_FILE_ATTRIBUTES) { MessageBoxW(h, L"还没有保存过速记。", L"速记", MB_OK | MB_ICONINFORMATION); return 0; }
            OPENFILENAMEW o;
            ZeroMemory(&o, sizeof o);
            o.lStructSize = sizeof o; o.hwndOwner = h;
            o.lpstrFilter = L"文本文件 (*.txt)\0*.txt\0";
            o.lpstrFile = dst; o.nMaxFile = MAX_PATH; o.lpstrDefExt = L"txt";
            o.Flags = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;
            if (GetSaveFileNameW(&o)) CopyFileW(src, dst, FALSE);
        } else if (id == 7) {
            WCHAR f[MAX_PATH];
            PathCombineW(f, g_noteDir, L"速记.txt");
            if (GetFileAttributesW(f) != INVALID_FILE_ATTRIBUTES) ShellExecuteW(NULL, L"open", f, NULL, NULL, SW_SHOWNORMAL);
        } else if (id >= 3 && id <= 6) NotePickDir(h, id);
        return 0;
    }
    case WM_CLOSE:
        GetWindowTextW(g_noteEdit, g_noteDraft, 16383);
        DestroyWindow(h);
        return 0;
    case WM_DESTROY: g_noteWnd = NULL; g_noteEdit = NULL; g_noteInfo = NULL; return 0;
    }
    return DefWindowProcW(h, m, w, l);
}

static void ShowNote(void) {
    if (g_noteWnd) { ShowWindow(g_noteWnd, SW_SHOWNORMAL); SetForegroundWindow(g_noteWnd); return; }
    NoteDirLoad();
    int W = Sc(420), H = Sc(340);
    RECT r = { 0, 0, W, H };
    AdjustWindowRect(&r, WS_CAPTION | WS_SYSMENU, FALSE);
    RECT wa = g_mi.rcWork;
    g_noteWnd = CreateWindowExW(WS_EX_TOPMOST, CLS_NOTE, L"速记", WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
                                (wa.left + wa.right - (r.right - r.left)) / 2, wa.bottom - (r.bottom - r.top) - Sc(120), r.right - r.left, r.bottom - r.top, NULL, NULL, g_inst, NULL);
    if (g_noteWnd) SetForegroundWindow(g_noteWnd);
}

/* ======================= icon dialog (file or link, fit mode, preview) ======================= */
typedef struct { HWND hw, e; WCHAR text[1024]; int fit, ok, reset, done; GpBitmap *prev; } IcoCtx;

static void IcoPreview(IcoCtx *c) {
    if (c->prev) { GdipDisposeImage(c->prev); c->prev = NULL; }
    WCHAR t[1024];
    GetWindowTextW(c->e, t, 1024);
    StrTrimW(t, L" \t\"");
    if (t[0] && !IsUrlStr(t) && GetFileAttributesW(t) != INVALID_FILE_ATTRIBUTES) {
        GpBitmap *s = NULL;
        if (GdipCreateBitmapFromFile(t, &s) == 0 && s) { c->prev = FitBitmap(s, 112, c->fit, 0.94f); GdipDisposeImage(s); }
    }
    RECT rc = { Sc(380), Sc(150), Sc(380) + 120, Sc(150) + 120 };
    InvalidateRect(c->hw, &rc, TRUE);
}

static LRESULT CALLBACK IcoProc(HWND h, UINT m, WPARAM w, LPARAM l) {
    IcoCtx *c = (IcoCtx *)GetWindowLongPtrW(h, GWLP_USERDATA);
    switch (m) {
    case WM_CREATE: {
        c = (IcoCtx *)((CREATESTRUCTW *)l)->lpCreateParams;
        SetWindowLongPtrW(h, GWLP_USERDATA, (LONG_PTR)c);
        c->hw = h;
        CreateWindowW(L"STATIC", L"图标图片（本地文件路径，或图片链接）：", WS_CHILD | WS_VISIBLE, Sc(16), Sc(14), Sc(480), Sc(20), h, NULL, g_inst, NULL);
        c->e = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", c->text, WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL, Sc(16), Sc(38), Sc(400), Sc(26), h, (HMENU)1, g_inst, NULL);
        CreateWindowW(L"BUTTON", L"浏览…", WS_CHILD | WS_VISIBLE | WS_TABSTOP, Sc(424), Sc(37), Sc(80), Sc(28), h, (HMENU)2, g_inst, NULL);
        CreateWindowW(L"STATIC", L"建议：正方形 PNG（透明背景），512×512 以上效果最佳。\n图片会自动缩放到 Dock 图标大小；选择“自动”可去掉多余的黑边 / 白边 / 透明边。", WS_CHILD | WS_VISIBLE, Sc(16), Sc(76), Sc(488), Sc(44), h, NULL, g_inst, NULL);
        CreateWindowW(L"STATIC", L"适配方式：", WS_CHILD | WS_VISIBLE, Sc(16), Sc(132), Sc(200), Sc(20), h, NULL, g_inst, NULL);
        static const WCHAR *fits[3] = { L"自动（去除透明 / 纯色边，推荐）", L"完整保留（不裁切，可能有留白）", L"居中裁切并填满（圆角方形）" };
        for (int i = 0; i < 3; i++) {
            HWND r = CreateWindowW(L"BUTTON", fits[i], WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_TABSTOP | (i == 0 ? WS_GROUP : 0), Sc(16), Sc(156) + i * Sc(30), Sc(330), Sc(26), h, (HMENU)(INT_PTR)(11 + i), g_inst, NULL);
            if (i == c->fit) SendMessageW(r, BM_SETCHECK, BST_CHECKED, 0);
        }
        CreateWindowW(L"BUTTON", L"确定", WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_DEFPUSHBUTTON, Sc(260), Sc(264), Sc(76), Sc(30), h, (HMENU)IDOK, g_inst, NULL);
        CreateWindowW(L"BUTTON", L"恢复默认图标", WS_CHILD | WS_VISIBLE | WS_TABSTOP, Sc(344), Sc(264), Sc(100), Sc(30), h, (HMENU)20, g_inst, NULL);
        CreateWindowW(L"BUTTON", L"取消", WS_CHILD | WS_VISIBLE | WS_TABSTOP, Sc(452), Sc(264), Sc(60), Sc(30), h, (HMENU)IDCANCEL, g_inst, NULL);
        SetFontAll(h);
        SetFocus(c->e);
        IcoPreview(c);
        return 0;
    }
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC dc = BeginPaint(h, &ps);
        GpGraphics *g = NULL;
        GdipCreateFromHDC(dc, &g);
        if (g) {
            SetupG(g);
            float px = (float)Sc(380), py = (float)Sc(150);
            FillRR(g, ARGBC(255, 232, 234, 240), px, py, 120, 120, 14);
            if (c && c->prev) DrawImg(g, c->prev, px + 4, py + 4, 112, 112, FALSE);
            else DrawCentered(g, L"预览", g_font, px, py, 120, 120, ARGBC(255, 140, 143, 155));
            GdipDeleteGraphics(g);
        }
        EndPaint(h, &ps);
        return 0;
    }
    case WM_COMMAND: {
        int id = LOWORD(w);
        if (id == 1 && HIWORD(w) == EN_CHANGE) IcoPreview(c);
        else if (id >= 11 && id <= 13) { c->fit = id - 11; IcoPreview(c); }
        else if (id == 2) {
            WCHAR f[MAX_PATH] = L"";
            OPENFILENAMEW o;
            ZeroMemory(&o, sizeof o);
            o.lStructSize = sizeof o; o.hwndOwner = h;
            o.lpstrFilter = L"图片 / 图标\0*.png;*.ico;*.jpg;*.jpeg;*.bmp;*.gif\0所有文件\0*.*\0";
            o.lpstrFile = f; o.nMaxFile = MAX_PATH; o.Flags = OFN_FILEMUSTEXIST;
            if (GetOpenFileNameW(&o)) SetWindowTextW(c->e, f);
        } else if (id == IDOK) { GetWindowTextW(c->e, c->text, 1024); c->ok = 1; c->done = 1; DestroyWindow(h); }
        else if (id == 20) { c->reset = 1; c->ok = 1; c->done = 1; DestroyWindow(h); }
        else if (id == IDCANCEL) { c->done = 1; DestroyWindow(h); }
        return 0;
    }
    case WM_CLOSE: c->done = 1; DestroyWindow(h); return 0;
    }
    return DefWindowProcW(h, m, w, l);
}

static void ShowIconDialog(HWND parent, int i) {
    if (i < 0 || i >= nItems) return;
    Item *it = &items[i];
    IcoCtx c;
    ZeroMemory(&c, sizeof c);
    c.fit = it->fit;
    lstrcpynW(c.text, it->icon_src, 1024);
    RECT wa = g_mi.rcWork;
    RECT r = { 0, 0, Sc(520), Sc(308) };
    AdjustWindowRectEx(&r, WS_CAPTION | WS_SYSMENU, FALSE, WS_EX_TOPMOST);
    HWND h = CreateWindowExW(WS_EX_TOPMOST | WS_EX_DLGMODALFRAME, CLS_ICOD, L"设置图标", WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
                             (wa.left + wa.right - (r.right - r.left)) / 2, (wa.top + wa.bottom - (r.bottom - r.top)) / 2 - Sc(40), r.right - r.left, r.bottom - r.top, parent, NULL, g_inst, &c);
    if (!h) return;
    if (parent) EnableWindow(parent, FALSE);
    SetForegroundWindow(h);
    MSG msg;
    while (!c.done && GetMessageW(&msg, NULL, 0, 0) > 0)
        if (!IsDialogMessageW(h, &msg)) { TranslateMessage(&msg); DispatchMessageW(&msg); }
    if (parent) { EnableWindow(parent, TRUE); SetForegroundWindow(parent); }
    if (c.prev) GdipDisposeImage(c.prev);
    if (!c.ok || i >= nItems) return;
    it = &items[i];
    StrTrimW(c.text, L" \t\"");
    if (c.reset) { it->icon_src[0] = 0; it->fit = 0; LoadItemIcon(it); }
    else if (c.text[0] && lstrcmpiW(c.text, it->icon_src) != 0) {
        it->fit = c.fit;
        WCHAR dst[MAX_PATH], nm[64];
        if (IsUrlStr(c.text)) {
            wsprintfW(nm, L"c_%08x.png", Fnv(c.text) ^ GetTickCount());
            PathCombineW(dst, g_iconDir, nm);
            lstrcpynW(it->icon_src, dst, 512);
            it->fav = 0;
            StartDownload(1, c.text, dst);
        } else if (GetFileAttributesW(c.text) != INVALID_FILE_ATTRIBUTES) {
            wsprintfW(nm, L"c_%08x%s", Fnv(c.text) ^ GetTickCount(), PathFindExtensionW(c.text));
            PathCombineW(dst, g_iconDir, nm);
            if (CopyFileW(c.text, dst, FALSE)) { lstrcpynW(it->icon_src, dst, 512); LoadItemIcon(it); }
        }
    } else if (it->icon_src[0] && c.fit != it->fit) { it->fit = c.fit; LoadItemIcon(it); }
    ScheduleSave();
    Kick();
}

/* ======================= minimize-to-dock animation ======================= */
static HWINEVENTHOOK g_minHook;
static HWND g_anim;
static HTHUMBNAIL g_animTh;
static RECT g_animFrom, g_animTo;
static DWORD g_animT0;
static int g_animItem = -1;

static void AnimStop(void) {
    KillTimer(g_hwnd, T_MIN);
    if (g_animTh) { DwmUnregisterThumbnail(g_animTh); g_animTh = NULL; }
    if (g_anim) { DestroyWindow(g_anim); g_anim = NULL; }
    if (g_animItem >= 0 && g_animItem < nItems && cfg.bounce) { items[g_animItem].bounce = GetTickCount() | 1; Kick(); }
    g_animItem = -1;
}

static void AnimStep(void) {
    if (!g_anim) return;
    float t = (GetTickCount() - g_animT0) / 280.f;
    if (t >= 1.f) { AnimStop(); return; }
    float e = t * t * (3 - 2 * t) * 0.6f + t * t * 0.4f;
    int fw = g_animFrom.right - g_animFrom.left, fh = g_animFrom.bottom - g_animFrom.top;
    int tw = g_animTo.right - g_animTo.left, th = g_animTo.bottom - g_animTo.top;
    int w = (int)(fw + (tw - fw) * e), h = (int)(fh + (th - fh) * e);
    int cx0 = (g_animFrom.left + g_animFrom.right) / 2, cy0 = (g_animFrom.top + g_animFrom.bottom) / 2;
    int cx1 = (g_animTo.left + g_animTo.right) / 2, cy1 = (g_animTo.top + g_animTo.bottom) / 2;
    int cx = (int)(cx0 + (cx1 - cx0) * e), cy = (int)(cy0 + (cy1 - cy0) * e);
    if (w < 8) w = 8;
    if (h < 8) h = 8;
    SetWindowPos(g_anim, HWND_TOPMOST, cx - w / 2, cy - h / 2, w, h, SWP_NOACTIVATE);
    DWM_THUMBNAIL_PROPERTIES tp;
    ZeroMemory(&tp, sizeof tp);
    tp.dwFlags = DWM_TNP_RECTDESTINATION | DWM_TNP_OPACITY | DWM_TNP_VISIBLE;
    tp.rcDestination.right = w; tp.rcDestination.bottom = h;
    tp.opacity = (BYTE)(255 * (1.f - e * 0.75f));
    tp.fVisible = TRUE;
    DwmUpdateThumbnailProperties(g_animTh, &tp);
}

static void CALLBACK MinProc(HWINEVENTHOOK hk, DWORD ev, HWND w, LONG idObj, LONG idChild, DWORD tid, DWORD ms) {
    if (ev != EVENT_SYSTEM_MINIMIZESTART || idObj != OBJID_WINDOW || !w || g_anim || !cfg.minAnim || gHideCur > 0.5f) return;
    if (!IsAppWindow(w) || w == g_hwnd || w == g_hset) return;
    WCHAR exe[MAX_PATH];
    if (!ExeOfWindow(w, exe)) return;
    int idx = -1;
    for (int i = 0; i < nItems && idx < 0; i++) {
        if (items[i].target[0] && lstrcmpiW(items[i].target, exe) == 0) idx = i;
        for (int k = 0; k < items[i].nk && idx < 0; k++) if (items[i].kids[k].target[0] && lstrcmpiW(items[i].kids[k].target, exe) == 0) idx = i;
    }
    if (idx < 0) return;
    RECT r;
    if (FAILED(DwmGetWindowAttribute(w, DWMWA_EXTENDED_FRAME_BOUNDS, &r, sizeof r))) GetWindowRect(w, &r);
    if (r.right - r.left < 40 || r.bottom - r.top < 40) return;
    g_animFrom = r;
    IconScreenRect(idx, &g_animTo);
    g_anim = CreateWindowExW(WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE | WS_EX_TRANSPARENT, CLS_ANIM, L"", WS_POPUP, r.left, r.top, r.right - r.left, r.bottom - r.top, NULL, NULL, g_inst, NULL);
    if (!g_anim) return;
    if (FAILED(DwmRegisterThumbnail(g_anim, w, &g_animTh))) { DestroyWindow(g_anim); g_anim = NULL; return; }
    g_animItem = idx;
    g_animT0 = GetTickCount();
    ShowWindow(g_anim, SW_SHOWNA);
    AnimStep();
    SetTimer(g_hwnd, T_MIN, 15, NULL);
}

static void SetMinHook(BOOL on) {
    if (on && !g_minHook) g_minHook = SetWinEventHook(EVENT_SYSTEM_MINIMIZESTART, EVENT_SYSTEM_MINIMIZESTART, NULL, MinProc, 0, 0, WINEVENT_OUTOFCONTEXT);
    else if (!on && g_minHook) { UnhookWinEvent(g_minHook); g_minHook = NULL; AnimStop(); }
}

/* ======================= add helpers ======================= */
enum {
    IDM_OPEN = 100, IDM_QUITAPP, IDM_SHOWFOLDER, IDM_WEBAPP, IDM_RENAME, IDM_REMOVE, IDM_ADDSEP_AFTER, IDM_SETTINGS, IDM_EXIT,
    IDM_AUTOSTART, IDM_ZOOM, IDM_ADDURL, IDM_ADDFILE, IDM_ADDFOLDER, IDM_ADDSEP_END, IDM_DELSEP, IDM_PIN, IDM_ICON_FILE,
    IDM_ICON_URL, IDM_ICON_RESET, IDM_UNGROUP, IDM_EMPTYBIN, IDM_ADDGROUP, IDM_ADDTRASH, IDM_NEWINST, IDM_LOCK, IDM_HIDE,
    IDM_SETICON, IDM_NEWSC, IDM_FOLDERFAN, IDM_NOTES, IDM_OPENFOLDER, IDM_NOTEOFF
};

static int CacheFor(int z) { return z <= 96 ? 96 : z <= 128 ? 128 : z <= 192 ? 192 : 256; }

static void DoAddUrl(HWND parent) {
    WCHAR url[1024] = L"https://", name[128] = L"";
    if (!InputBox(parent, L"添加网页应用", L"网址：", url, 1024, L"名称（可留空，默认使用域名）：", name, 128)) return;
    StrTrimW(url, L" \t");
    if (!url[0]) return;
    if (!IsUrlStr(url)) { WCHAR t[1024]; wsprintfW(t, L"https://%s", url); lstrcpyW(url, t); }
    int r = AddFromString(url, -1);
    if (r >= 0 && name[0]) { lstrcpynW(items[r].name, name, 128); CleanName(items[r].name); ScheduleSave(); }
    UpdateGeometry();
    Kick();
}

static void DoNewShortcut(HWND parent) {
    WCHAR loc[1024] = L"", name[128] = L"";
    if (!InputBox(parent, L"新建快捷方式", L"位置（应用路径、文件夹或网址，.exe 后可带启动参数）：", loc, 1024, L"名称（可留空）：", name, 128)) return;
    StrTrimW(loc, L" \t\"");
    if (!loc[0]) return;
    WCHAR args[260] = L"";
    int r = -1;
    if (IsUrlStr(loc)) r = AddFromString(loc, -1);
    else {
        WCHAR *p = StrStrIW(loc, L".exe");
        if (p && p[4] && p[4] == L' ') { lstrcpynW(args, p + 5, 260); p[4] = 0; StrTrimW(args, L" "); }
        else if (p && p[4] == 0) args[0] = 0;
        r = AddFromString(loc, -1);
        if (r >= 0 && args[0]) lstrcpynW(items[r].args, args, 260);
    }
    if (r < 0) { MessageBoxW(parent, L"找不到这个位置，请检查路径或网址是否正确。", L"MiniDock", MB_OK | MB_ICONWARNING); return; }
    if (name[0]) { lstrcpynW(items[r].name, name, 128); CleanName(items[r].name); }
    ScheduleSave();
    UpdateGeometry();
    Kick();
}

static void DoAddFile(HWND parent) {
    WCHAR f[MAX_PATH] = L"";
    OPENFILENAMEW o;
    ZeroMemory(&o, sizeof o);
    o.lStructSize = sizeof o; o.hwndOwner = parent;
    o.lpstrFilter = L"应用程序与快捷方式\0*.exe;*.lnk;*.url\0所有文件\0*.*\0";
    o.lpstrFile = f; o.nMaxFile = MAX_PATH;
    o.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_NODEREFERENCELINKS;
    o.lpstrTitle = L"选择要固定到 Dock 的应用或文件";
    if (GetOpenFileNameW(&o)) { AddFromString(f, -1); UpdateGeometry(); Kick(); }
}

static void DoAddFolder(HWND parent) {
    BROWSEINFOW b;
    ZeroMemory(&b, sizeof b);
    b.hwndOwner = parent;
    b.lpszTitle = L"选择要固定到 Dock 的文件夹";
    b.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
    LPITEMIDLIST pi = SHBrowseForFolderW(&b);
    if (pi) {
        WCHAR p[MAX_PATH];
        if (SHGetPathFromIDListW(pi, p)) { AddFromString(p, -1); UpdateGeometry(); Kick(); }
        CoTaskMemFree(pi);
    }
}

static void DoAddSep(int idx) {
    Item s;
    FillItem(&s, T_SEP, 0, L"", L"", NULL);
    InsertItemStruct(idx, &s, FALSE);
    ScheduleSave();
    UpdateGeometry();
    Kick();
}

static void CopyCustomIcon(int i, HWND parent) {
    WCHAR f[MAX_PATH] = L"";
    OPENFILENAMEW o;
    ZeroMemory(&o, sizeof o);
    o.lStructSize = sizeof o; o.hwndOwner = parent;
    o.lpstrFilter = L"图片 / 图标\0*.png;*.ico;*.jpg;*.jpeg;*.bmp;*.gif\0所有文件\0*.*\0";
    o.lpstrFile = f; o.nMaxFile = MAX_PATH; o.Flags = OFN_FILEMUSTEXIST;
    o.lpstrTitle = L"选择图标图片（会自动缩放到 Dock 图标大小）";
    if (!GetOpenFileNameW(&o)) return;
    WCHAR dst[MAX_PATH], nm[64];
    wsprintfW(nm, L"c_%08x%s", Fnv(f) ^ GetTickCount(), PathFindExtensionW(f));
    PathCombineW(dst, g_iconDir, nm);
    if (!CopyFileW(f, dst, FALSE)) return;
    lstrcpynW(items[i].icon_src, dst, 512);
    LoadItemIcon(&items[i]);
    ScheduleSave();
    Kick();
}

static void LinkCustomIcon(int i, HWND parent) {
    WCHAR url[1024] = L"https://";
    if (!InputBox(parent, L"自定义图标", L"图标图片链接（png / ico / jpg，会自动缩放适配）：", url, 1024, NULL, NULL, 0)) return;
    StrTrimW(url, L" \t");
    if (!IsUrlStr(url)) return;
    WCHAR dst[MAX_PATH], nm[64];
    wsprintfW(nm, L"c_%08x.png", Fnv(url) ^ GetTickCount());
    PathCombineW(dst, g_iconDir, nm);
    lstrcpynW(items[i].icon_src, dst, 512);
    items[i].fav = 0;
    StartDownload(1, url, dst);
    ScheduleSave();
}

/* ======================= menus ======================= */
static void ToggleHide(void) { gForceHide = !gForceHide; Kick(); }

static int AddCommon(CmItem *m, int n) {
    n = CmAddH(m, n, IDM_SETTINGS, L"Dock 设置…", 0, L"");
    n = CmAdd(m, n, IDM_ZOOM, L"悬停放大", cfg.zoom ? CM_CHECK : 0);
    n = CmAdd(m, n, IDM_LOCK, L"锁定 Dock 布局", cfg.lock ? CM_CHECK : 0);
    n = CmAdd(m, n, IDM_AUTOSTART, L"开机自启动", GetAutostart() ? CM_CHECK : 0);
    n = CmAddH(m, n, IDM_HIDE, L"隐藏 / 显示 Dock", 0, L"Ctrl+Alt+D");
    n = CmAdd(m, n, IDM_NOTES, L"速记工具", cfg.notes ? CM_CHECK : 0);
    n = CmAdd(m, n, 0, L"", CM_SEP);
    n = CmAdd(m, n, IDM_EXIT, L"退出 Dock", CM_RED);
    return n;
}
static int AddAddMenu(CmItem *m, int n) {
    n = CmAdd(m, n, IDM_ADDURL, L"添加网页应用…", 0);
    n = CmAdd(m, n, IDM_ADDFILE, L"添加应用 / 文件…", 0);
    n = CmAdd(m, n, IDM_ADDFOLDER, L"添加文件夹…", 0);
    n = CmAdd(m, n, IDM_NEWSC, L"新建快捷方式（应用链接 / 网址）…", 0);
    n = CmAdd(m, n, IDM_ADDGROUP, L"新建应用组", 0);
    n = CmAdd(m, n, IDM_ADDTRASH, L"添加回收站", 0);
    n = CmAdd(m, n, IDM_ADDSEP_END, L"在末尾添加分割线", 0);
    n = CmAdd(m, n, 0, L"", CM_SEP);
    return n;
}

static void HandleCmd(int id) {
    int i = g_menuItem;
    switch (id) {
    case IDM_OPEN: LaunchItem(i, FALSE); break;
    case IDM_NEWINST: LaunchItem(i, TRUE); break;
    case IDM_QUITAPP: if (i >= 0 && items[i].target[0]) ActivateApp(items[i].target, TRUE); break;
    case IDM_SHOWFOLDER:
        if (i >= 0) {
            WCHAR a[1100];
            wsprintfW(a, L"/select,\"%s\"", items[i].path);
            ShellExecuteW(NULL, L"open", L"explorer.exe", a, NULL, SW_SHOWNORMAL);
        }
        break;
    case IDM_WEBAPP: if (i >= 0) { items[i].appmode = !items[i].appmode; ScheduleSave(); } break;
    case IDM_RENAME:
        if (i >= 0) {
            WCHAR nm[128];
            lstrcpynW(nm, items[i].name, 128);
            if (InputBox(g_hset, L"重命名", L"名称：", nm, 128, NULL, NULL, 0) && nm[0]) {
                lstrcpynW(items[i].name, nm, 128);
                CleanName(items[i].name);
                ScheduleSave();
                Kick();
            }
        }
        break;
    case IDM_REMOVE:
    case IDM_DELSEP: if (i >= 0) RemoveItem(i); UpdateGeometry(); break;
    case IDM_UNGROUP: DissolveGroup(i); UpdateGeometry(); break;
    case IDM_EMPTYBIN: SHEmptyRecycleBinW(g_hwnd, NULL, 0); g_trashFull = -1; SetTimer(g_hwnd, T_SCAN, 500, NULL); break;
    case IDM_PIN:
        if (i >= 0 && items[i].temp) {
            int ts = PinnedCount();
            MoveItem(i, ts);
            items[ts].temp = 0;
            ScheduleSave();
            SetTimer(g_hwnd, T_SCAN, 300, NULL);
            Kick();
        }
        break;
    case IDM_SETICON: if (i >= 0) ShowIconDialog(g_hset, i); break;
    case IDM_NEWSC: DoNewShortcut(g_hset); break;
    case IDM_FOLDERFAN: cfg.folderFan = !cfg.folderFan; ApplySettings(); break;
    case IDM_OPENFOLDER: if (i >= 0) LaunchItem(i, TRUE); break;
    case IDM_NOTES: cfg.notes = !cfg.notes; ApplySettings(); break;
    case IDM_NOTEOFF: cfg.notes = 0; ApplySettings(); break;
    case IDM_ADDSEP_AFTER: DoAddSep(i >= 0 ? i + 1 : -1); break;
    case IDM_ADDSEP_END: DoAddSep(-1); break;
    case IDM_ADDURL: DoAddUrl(g_hset); break;
    case IDM_ADDFILE: DoAddFile(g_hset); break;
    case IDM_ADDFOLDER: DoAddFolder(g_hset); break;
    case IDM_ADDGROUP: NewEmptyGroup(); UpdateGeometry(); Kick(); break;
    case IDM_ADDTRASH: AddTrash(); UpdateGeometry(); Kick(); break;
    case IDM_SETTINGS: ShowSettings(0); break;
    case IDM_ZOOM: cfg.zoom = !cfg.zoom; ApplySettings(); break;
    case IDM_LOCK: cfg.lock = !cfg.lock; ApplySettings(); break;
    case IDM_HIDE: ToggleHide(); break;
    case IDM_AUTOSTART: SetAutostart(!GetAutostart()); break;
    case IDM_EXIT: DestroyWindow(g_hwnd); break;
    }
}

static void ShowMenuAt(int item, BOOL fromTray) {
    CmItem m[28];
    int n = 0;
    g_menuItem = item;
    if (item >= 0 && !fromTray) {
        Item *it = &items[item];
        if (it->type == T_SEP) {
            if (!it->temp) { n = CmAdd(m, n, IDM_DELSEP, L"删除分割线", 0); n = CmAdd(m, n, 0, L"", CM_SEP); }
        } else if (it->type == T_GROUP) {
            n = CmAdd(m, n, IDM_OPEN, L"打开", 0);
            n = CmAdd(m, n, IDM_RENAME, L"重命名…", 0);
            n = CmAdd(m, n, IDM_UNGROUP, L"解散应用组", 0);
            n = CmAdd(m, n, IDM_REMOVE, L"从 Dock 移除", CM_RED);
            n = CmAdd(m, n, 0, L"", CM_SEP);
        } else if (it->type == T_NOTE) {
            n = CmAdd(m, n, IDM_OPEN, L"打开速记", 0);
            n = CmAdd(m, n, IDM_NOTEOFF, L"关闭速记工具", CM_RED);
            n = CmAdd(m, n, 0, L"", CM_SEP);
        } else if (it->type == T_TRASH) {
            n = CmAdd(m, n, IDM_OPEN, L"打开回收站", 0);
            n = CmAdd(m, n, IDM_EMPTYBIN, L"清空回收站", 0);
            n = CmAdd(m, n, IDM_REMOVE, L"从 Dock 移除", CM_RED);
            n = CmAdd(m, n, 0, L"", CM_SEP);
        } else {
            n = CmAdd(m, n, IDM_OPEN, it->running ? L"切换到此应用" : L"打开", 0);
            if (it->running && it->type == T_FILE) n = CmAdd(m, n, IDM_NEWINST, L"新建窗口 / 实例", 0);
            if (it->running && it->target[0]) n = CmAdd(m, n, IDM_QUITAPP, L"退出应用", 0);
            n = CmAdd(m, n, 0, L"", CM_SEP);
            if (it->type == T_FOLDER) {
                n = CmAdd(m, n, IDM_OPENFOLDER, L"在资源管理器中打开", 0);
                n = CmAdd(m, n, IDM_FOLDERFAN, L"点击时弧形堆叠展开", cfg.folderFan ? CM_CHECK : 0);
            }
            if (it->type == T_URL) n = CmAdd(m, n, IDM_WEBAPP, L"以应用窗口打开（Edge / Chrome）", it->appmode ? CM_CHECK : 0);
            else n = CmAdd(m, n, IDM_SHOWFOLDER, L"在文件夹中显示", 0);
            if (it->temp) n = CmAdd(m, n, IDM_PIN, L"固定到 Dock", 0);
            else {
                n = CmAdd(m, n, IDM_RENAME, L"重命名…", 0);
                n = CmAdd(m, n, IDM_SETICON, L"设置图标…", 0);
                n = CmAdd(m, n, IDM_ADDSEP_AFTER, L"在右侧添加分割线", 0);
                n = CmAdd(m, n, IDM_REMOVE, L"从 Dock 移除", CM_RED);
            }
            n = CmAdd(m, n, 0, L"", CM_SEP);
        }
    } else n = AddAddMenu(m, n);
    n = AddCommon(m, n);
    POINT p;
    GetCursorPos(&p);
    gMenuOpen = TRUE;
    ClosePreview();
    int cmd = ShowMenu(m, n, p.x, p.y);
    gMenuOpen = FALSE;
    gLastNear = GetTickCount();
    if (cmd) HandleCmd(cmd);
    g_menuItem = -1;
    Kick();
}


/* ======================= settings window: custom drawn, double buffered ======================= */
enum { R_HDR, R_TOGGLE, R_SLIDER, R_COMBO, R_BTN, R_BTN2, R_COLOR, R_TEXT, R_ITEM, R_STAT };
enum {
    A_NONE = 0, A_AUTOSTART, A_FROST, A_SCAN, A_ADDURL, A_ADDFILE, A_ADDFOLDER, A_NEWSC, A_NEWGROUP, A_ADDTRASH, A_ADDSEP,
    A_EXP0, A_EXP1, A_EXP2, A_IMPORT, A_RESET, A_OPENDIR, A_LOWMODE, A_RECMODE, A_PICKDIR, A_AUTOMATCH, A_ITEM = 1000
};
typedef struct {
    int type, page, act, act2, mn, mx, nopts;
    const WCHAR *label, *label2, *suffix;
    int *v;
    const WCHAR **opts;
    float y, h;
    WCHAR dyn[200];
} Row;

static Row g_row[260];
static int g_nrow, g_page, g_sbHot = -1, g_rowHot = -1, g_rowDrag = -1, g_scroll;
static HBITMAP g_sbmp, g_sold;
static HDC g_sdc;
static int g_sw, g_sh;
static GpFont *g_fontTitle;
static GpStringFormat *g_fmtW;
#define SBW 208
#define NPAGE 10
static const WCHAR *g_pageName[NPAGE] = { L"通用", L"外观", L"位置与模式", L"图标样式", L"行为与动画", L"小组件", L"性能与资源", L"图标批量设置", L"项目与配置", L"关于" };

static WCHAR g_batchDir[MAX_PATH];
static WCHAR g_match[MAXI][MAX_PATH];
static int g_mstate[MAXI];

static const WCHAR *o_theme[3] = { L"深色", L"浅色", L"跟随系统" };
static const WCHAR *o_bg[3] = { L"纯色", L"渐变（Material You）", L"磨砂玻璃（模糊背景）" };
static const WCHAR *o_edge[4] = { L"屏幕底部", L"屏幕顶部", L"屏幕左侧", L"屏幕右侧" };
static const WCHAR *o_mode[3] = { L"常驻显示", L"自动隐藏", L"智能隐藏（被窗口遮挡时隐藏）" };
static const WCHAR *o_plate[5] = { L"无底盘", L"浅色实心", L"深色实心", L"浅色半透明", L"深色半透明" };
static const WCHAR *o_dot[2] = { L"横条", L"圆点" };
static WCHAR o_monN[8][24];
static const WCHAR *o_mon[8];
static int g_nmonOpt;

static Row *NewRow(int page, int type, const WCHAR *label, int h96) {
    Row *r = &g_row[g_nrow++];
    ZeroMemory(r, sizeof *r);
    r->page = page; r->type = type; r->label = label; r->h = (float)Sc(h96);
    return r;
}
static void RHdr(int p, const WCHAR *t) { NewRow(p, R_HDR, t, 40); }
static void RTog(int p, const WCHAR *t, int *v, const WCHAR *note) { Row *r = NewRow(p, R_TOGGLE, t, note ? 56 : 46); r->v = v; r->label2 = note; }
static void RTogA(int p, const WCHAR *t, int act, const WCHAR *note) { Row *r = NewRow(p, R_TOGGLE, t, note ? 56 : 46); r->act = act; r->label2 = note; }
static void RSl(int p, const WCHAR *t, int *v, int mn, int mx, const WCHAR *suf) { Row *r = NewRow(p, R_SLIDER, t, 48); r->v = v; r->mn = mn; r->mx = mx; r->suffix = suf; }
static void RCmb(int p, const WCHAR *t, int *v, const WCHAR **o, int n) { Row *r = NewRow(p, R_COMBO, t, 50); r->v = v; r->opts = o; r->nopts = n; }
static void RBtn(int p, const WCHAR *t, int act) { Row *r = NewRow(p, R_BTN, t, 46); r->act = act; }
static void RBtn2(int p, const WCHAR *t1, int a1, const WCHAR *t2, int a2) { Row *r = NewRow(p, R_BTN2, t1, 50); r->act = a1; r->label2 = t2; r->act2 = a2; }
static void RCol(int p, const WCHAR *t, int *v) { Row *r = NewRow(p, R_COLOR, t, 48); r->v = v; }
static void RTxt(int p, const WCHAR *t, int h96) { NewRow(p, R_TEXT, t, h96); }

static BOOL GetSpecial(int act) {
    if (act == A_AUTOSTART) return GetAutostart();
    if (act == A_FROST) return cfg.bgMode == 2;
    if (act == A_SCAN) return cfg.dots || cfg.showRunning;
    return FALSE;
}
static void SetSpecial(int act, BOOL on) {
    if (act == A_AUTOSTART) SetAutostart(on);
    else if (act == A_FROST) cfg.bgMode = on ? 2 : 0;
    else if (act == A_SCAN) { cfg.dots = on; cfg.showRunning = on; }
}

/* ---- batch icon replacement ---- */
static void NormName(const WCHAR *s, WCHAR *o) {
    int n = 0;
    for (; *s && n < 120; s++) if (*s != L' ' && *s != L'_' && *s != L'-' && *s != L'.') o[n++] = *s;
    o[n] = 0;
    CharLowerW(o);
}

static BOOL IsImgExt(const WCHAR *f) { return PathMatchSpecW(f, L"*.png;*.ico;*.jpg;*.jpeg;*.bmp;*.gif"); }

static void RunMatch(void) {
    for (int i = 0; i < MAXI; i++) { g_match[i][0] = 0; g_mstate[i] = 0; }
    if (!g_batchDir[0]) return;
    WCHAR pat[MAX_PATH];
    PathCombineW(pat, g_batchDir, L"*");
    for (int i = 0; i < nItems; i++) {
        if (items[i].type == T_SEP || items[i].temp) continue;
        WCHAR nn[130], fn[130];
        NormName(items[i].name, nn);
        WIN32_FIND_DATAW fd;
        HANDLE h = FindFirstFileW(pat, &fd);
        int best = 0;
        if (h != INVALID_HANDLE_VALUE) {
            do {
                if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) || !IsImgExt(fd.cFileName)) continue;
                WCHAR stem[MAX_PATH];
                lstrcpynW(stem, fd.cFileName, MAX_PATH);
                PathRemoveExtensionW(stem);
                NormName(stem, fn);
                int sc = 0;
                if (!lstrcmpW(fn, nn)) sc = 3;
                else if (nn[0] && fn[0] && (wcsstr(fn, nn) || wcsstr(nn, fn)) && lstrlenW(fn) >= 2) sc = 1;
                if (sc > best) { best = sc; PathCombineW(g_match[i], g_batchDir, fd.cFileName); }
            } while (FindNextFileW(h, &fd));
            FindClose(h);
        }
        g_mstate[i] = best ? 1 : 2;
    }
}

static BOOL ApplyIconFile(int i, const WCHAR *file) {
    WCHAR dst[MAX_PATH], nm[64];
    wsprintfW(nm, L"c_%08x%s", Fnv(file) ^ GetTickCount() ^ (i * 7919), PathFindExtensionW(file));
    PathCombineW(dst, g_iconDir, nm);
    if (!CopyFileW(file, dst, FALSE)) return FALSE;
    lstrcpynW(items[i].icon_src, dst, 512);
    items[i].fit = 0;
    LoadItemIcon(&items[i]);
    return TRUE;
}

/* ---- build rows ---- */
static void BuildRows(void) {
    g_nrow = 0;
    GetMon();
    g_nmonOpt = g_nmon < 1 ? 1 : g_nmon;
    for (int i = 0; i < g_nmonOpt; i++) { wsprintfW(o_monN[i], i == 0 ? L"主显示器" : L"显示器 %d", i + 1); o_mon[i] = o_monN[i]; }
    int P = 0;
    RHdr(P, L"启动与常规");
    RTogA(P, L"开机自启动", A_AUTOSTART, NULL);
    RTog(P, L"锁定 Dock 布局", &cfg.lock, L"防止误拖动、误移除图标");
    RTog(P, L"显示未固定的运行中应用", &cfg.showRunning, L"在分割线之后显示正在运行的应用，可拖入固定");
    RTog(P, L"全局快捷键 Ctrl + Alt + D", &cfg.hotkey, L"随时显示 / 隐藏 Dock");
    RTog(P, L"网页默认以应用窗口打开", &cfg.webApp, L"使用 Edge / Chrome 的 --app 模式");
    RHdr(P, L"设置界面");
    RCmb(P, L"设置窗口主题", &cfg.uiTheme, o_theme, 3);
    RHdr(P, L"数据");
    RBtn(P, L"打开配置文件夹", A_OPENDIR);
    NewRow(P, R_TEXT, g_dir, 30);

    P = 1;
    RHdr(P, L"尺寸");
    RSl(P, L"图标大小", &cfg.iconSize, 24, 128, L" px");
    RSl(P, L"放大后大小", &cfg.zoomSize, 32, 220, L" px");
    RSl(P, L"图标间距", &cfg.spacing, 0, 30, L" px");
    RSl(P, L"栏内边距（栏高）", &cfg.pad, 2, 30, L" px");
    RSl(P, L"栏圆角", &cfg.radius, 0, 40, L" px");
    RHdr(P, L"背景");
    RCmb(P, L"Dock 主题", &cfg.theme, o_theme, 3);
    RCmb(P, L"背景材质", &cfg.bgMode, o_bg, 3);
    RSl(P, L"不透明度", &cfg.opacity, 0, 100, L" %");
    RSl(P, L"磨砂模糊强度", &cfg.blur, 1, 14, L"");
    RTog(P, L"显示栏边框", &cfg.border, NULL);
    RTog(P, L"跟随系统强调色（纯色 / 渐变）", &cfg.accentTone, L"根据 Windows 强调色生成 Material You 风格色调");
    { Row *r = NewRow(P, R_COLOR, L"自定义颜色 1", 48); r->v = &cfg.bgColor1; }
    { Row *r = NewRow(P, R_COLOR, L"自定义颜色 2（渐变终点）", 48); r->v = &cfg.bgColor2; }

    P = 2;
    RHdr(P, L"位置");
    RCmb(P, L"Dock 位置（居中）", &cfg.edge, o_edge, 4);
    RCmb(P, L"显示在", &cfg.monitor, o_mon, g_nmonOpt);
    RSl(P, L"距屏幕边缘", &cfg.margin, 0, 150, L" px");
    RHdr(P, L"显示模式");
    RCmb(P, L"模式", &cfg.mode, o_mode, 3);
    RTog(P, L"最大化窗口避让 Dock", &cfg.reserve, L"仅“常驻显示”模式有效，像 macOS 一样为 Dock 预留桌面空间");

    P = 3;
    RHdr(P, L"图标底盘（macOS 风格圆角方块）");
    RCmb(P, L"浅色模式下的底盘", &cfg.plateLight, o_plate, 5);
    RCmb(P, L"深色模式下的底盘", &cfg.plateDark, o_plate, 5);
    RSl(P, L"底盘圆角", &cfg.plateRadius, 0, 50, L" %");
    RTxt(P, L"圆角 0% = 方形，22% = 默认圆角方块，50% = 圆形。", 30);
    RHdr(P, L"运行指示");
    RTog(P, L"显示运行中指示", &cfg.dots, NULL);
    RCmb(P, L"指示样式", &cfg.dotStyle, o_dot, 2);
    RHdr(P, L"标签");
    RTog(P, L"悬停显示名称标签", &cfg.labels, NULL);

    P = 4;
    RHdr(P, L"交互");
    RTog(P, L"悬停放大图标", &cfg.zoom, NULL);
    RTog(P, L"点击文件夹时弧形堆叠展开", &cfg.folderFan, L"像 macOS 一样列出最近的文件，点击即可打开");
    RHdr(P, L"动画");
    RTog(P, L"启动应用时弹跳", &cfg.bounce, NULL);
    RTog(P, L"最小化缩入 Dock 动画", &cfg.minAnim, L"窗口最小化时缩小飞入对应图标");
    RTog(P, L"悬停运行中应用显示窗口预览", &cfg.preview, NULL);

    P = 5;
    RHdr(P, L"Dock 附件");
    RTog(P, L"在 Dock 右侧显示时间与日期", &cfg.clock, L"每 30 秒刷新一次，占用极低");
    RTog(P, L"图标倒影", &cfg.reflect, L"仅底部停靠时有效");
    RHdr(P, L"小工具");
    RTog(P, L"速记（点击即可记录文字）", &cfg.notes, L"开启后 Dock 上出现“速记”图标，可保存 / 导出 txt，并自定义保存位置");

    P = 6;
    RHdr(P, L"当前占用");
    { Row *r = NewRow(P, R_STAT, L"内存（工作集 / 私有）", 46); (void)r; }
    RHdr(P, L"高耗能功能（可单独关闭）");
    RTogA(P, L"磨砂玻璃背景", A_FROST, L"中等：每 2.5 秒截取并模糊背景");
    RTogA(P, L"扫描运行中应用（指示点 / 运行中列表）", A_SCAN, L"中等：每 2.5 秒枚举窗口；关闭后几乎不做后台扫描");
    RTog(P, L"悬停窗口预览", &cfg.preview, L"低：仅悬停时使用系统缩略图");
    RTog(P, L"时间与日期", &cfg.clock, L"低：每 30 秒刷新一次");
    RTog(P, L"图标倒影", &cfg.reflect, L"低：多画一层半透明图像");
    RTog(P, L"最小化缩入动画", &cfg.minAnim, L"低：仅在窗口最小化时运行");
    RHdr(P, L"预设");
    RBtn2(P, L"一键低占用模式", A_LOWMODE, L"恢复推荐设置", A_RECMODE);
    RTxt(P, L"空闲时程序没有常驻动画定时器，图像只在交互时刷新；内存会在空闲 2.5 秒后自动回收到最低。", 44);

    P = 7;
    RHdr(P, L"从文件夹批量设置图标");
    RBtn2(P, L"选择图标文件夹…", A_PICKDIR, L"自动匹配并应用", A_AUTOMATCH);
    if (g_batchDir[0]) NewRow(P, R_TEXT, g_batchDir, 30);
    else RTxt(P, L"选择一个存放图标（png / ico / jpg）的文件夹，文件名与 Dock 上的应用名称相同即可自动匹配。也可以点击下面任意一行单独更换。", 56);
    RHdr(P, L"Dock 上的图标");
    for (int i = 0; i < nItems; i++) {
        if (items[i].type == T_SEP || items[i].temp || g_nrow > 250) continue;
        Row *r = NewRow(P, R_ITEM, NULL, 46);
        r->act = A_ITEM + i;
        lstrcpynW(r->dyn, items[i].name, 200);
        r->label = r->dyn;
    }

    P = 8;
    RHdr(P, L"添加");
    RBtn(P, L"添加网页应用…", A_ADDURL);
    RBtn(P, L"添加应用 / 文件…", A_ADDFILE);
    RBtn(P, L"添加文件夹…", A_ADDFOLDER);
    RBtn(P, L"新建快捷方式（应用链接 / 网址）…", A_NEWSC);
    RBtn(P, L"新建应用组", A_NEWGROUP);
    RBtn(P, L"添加回收站", A_ADDTRASH);
    RBtn(P, L"添加分割线", A_ADDSEP);
    RHdr(P, L"导入与导出");
    RBtn(P, L"导出完整配置…", A_EXP0);
    RBtn(P, L"仅导出设置参数…", A_EXP1);
    RBtn(P, L"仅导出应用与图标…", A_EXP2);
    RBtn(P, L"导入配置…", A_IMPORT);
    RHdr(P, L"重置");
    RBtn(P, L"恢复默认设置", A_RESET);

    P = 9;
    RTxt(P, L"ABOUT", 360);
}

/* ---- painting ---- */
static ARGB cBg, cSide, cCard, cLine, cText, cSub, cHover, cTrack;
static void UiColors(void) {
    if (g_uiDark) { cBg = ARGBC(255, 24, 25, 29); cSide = ARGBC(255, 32, 33, 38); cCard = ARGBC(255, 40, 41, 47); cLine = ARGBC(255, 58, 60, 68); cText = ARGBC(255, 236, 237, 242); cSub = ARGBC(255, 146, 150, 162); cHover = ARGBC(255, 50, 52, 60); cTrack = ARGBC(255, 78, 81, 92); }
    else { cBg = ARGBC(255, 243, 244, 248); cSide = ARGBC(255, 234, 236, 242); cCard = ARGBC(255, 255, 255, 255); cLine = ARGBC(255, 232, 234, 240); cText = ARGBC(255, 28, 30, 38); cSub = ARGBC(255, 122, 126, 140); cHover = ARGBC(255, 244, 246, 251); cTrack = ARGBC(255, 212, 216, 228); }
}

static float g_x0, g_cw, g_viewH, g_contentH;

static void RowRects(Row *r, RectF *a, RectF *b) {
    float xr = g_x0 + g_cw - Sc(16);
    a->X = a->Y = a->Width = a->Height = 0;
    *b = *a;
    switch (r->type) {
    case R_TOGGLE: a->X = xr - Sc(46); a->Y = r->y + (r->h - Sc(26)) / 2; a->Width = (float)Sc(46); a->Height = (float)Sc(26); break;
    case R_SLIDER:
        a->X = g_x0 + g_cw * 0.40f; a->Y = r->y; a->Width = xr - Sc(84) - a->X; a->Height = r->h;
        b->X = xr - Sc(72); b->Y = r->y + (r->h - Sc(30)) / 2; b->Width = (float)Sc(72); b->Height = (float)Sc(30); break;
    case R_COMBO: a->X = xr - Sc(250); a->Y = r->y + (r->h - Sc(34)) / 2; a->Width = (float)Sc(250); a->Height = (float)Sc(34); break;
    case R_BTN: case R_ITEM: a->X = g_x0; a->Y = r->y; a->Width = g_cw; a->Height = r->h; break;
    case R_BTN2:
        a->X = xr - Sc(300); a->Y = r->y + (r->h - Sc(34)) / 2; a->Width = (float)Sc(144); a->Height = (float)Sc(34);
        b->X = xr - Sc(148); b->Y = a->Y; b->Width = (float)Sc(148); b->Height = a->Height; break;
    case R_COLOR:
        a->X = xr - Sc(40); a->Y = r->y + (r->h - Sc(30)) / 2; a->Width = (float)Sc(40); a->Height = (float)Sc(30);
        b->X = xr - Sc(120); b->Y = a->Y; b->Width = (float)Sc(70); b->Height = a->Height; break;
    }
}

static BOOL InRect(const RectF *r, float x, float y) { return x >= r->X && x < r->X + r->Width && y >= r->Y && y < r->Y + r->Height; }

static void LayoutRows(void) {
    float y = (float)Sc(78) - g_scroll;
    for (int i = 0; i < g_nrow; i++) {
        if (g_row[i].page != g_page) continue;
        g_row[i].y = y;
        y += g_row[i].h;
    }
    g_contentH = y + g_scroll - Sc(78);
}

static void DrawToggle(GpGraphics *g, RectF *a, BOOL on) {
    FillRR(g, on ? ARGBC(255, 52, 199, 89) : cTrack, a->X, a->Y, a->Width, a->Height, a->Height / 2);
    float k = a->Height - 6;
    FillRR(g, ARGBC(255, 255, 255, 255), on ? a->X + a->Width - k - 3 : a->X + 3, a->Y + 3, k, k, k / 2);
}

static void DrawChevronDown(GpGraphics *g, float x, float y, ARGB c) {
    GpPen *p;
    GdipCreatePen1(c, 1.6f, 2, &p);
    GdipDrawLine(g, p, x - 4, y - 2, x, y + 2);
    GdipDrawLine(g, p, x, y + 2, x + 4, y - 2);
    GdipDeletePen(p);
}
static void DrawChevronRight(GpGraphics *g, float x, float y, ARGB c) {
    GpPen *p;
    GdipCreatePen1(c, 1.6f, 2, &p);
    GdipDrawLine(g, p, x - 2, y - 4, x + 2, y);
    GdipDrawLine(g, p, x + 2, y, x - 2, y + 4);
    GdipDeletePen(p);
}

static void PaintSettings(HDC dc, int W, int H) {
    UiColors();
    GpGraphics *g = NULL;
    GdipCreateFromHDC(dc, &g);
    if (!g) return;
    SetupG(g);
    GdipGraphicsClear(g, cBg);
    float sbw = (float)Sc(SBW);
    g_x0 = sbw + Sc(30);
    g_cw = W - g_x0 - Sc(30);
    g_viewH = (float)H - Sc(78);
    LayoutRows();
    /* content rows (clipped below the title) */
    for (int i = 0; i < g_nrow;) {
        if (g_row[i].page != g_page || g_row[i].type == R_HDR) { i++; continue; }
        int j = i;
        while (j + 1 < g_nrow && g_row[j + 1].page == g_page && g_row[j + 1].type != R_HDR) j++;
        float y0 = g_row[i].y, y1 = g_row[j].y + g_row[j].h;
        if (y1 > Sc(66) && y0 < H) { FillRR(g, cCard, g_x0, y0, g_cw, y1 - y0, 14); StrokeRR(g, cLine, 1, g_x0, y0, g_cw, y1 - y0, 14); }
        i = j + 1;
    }
    for (int i = 0; i < g_nrow; i++) {
        Row *r = &g_row[i];
        if (r->page != g_page || r->y + r->h < Sc(64) || r->y > H) continue;
        float xl = g_x0 + Sc(18), xr = g_x0 + g_cw - Sc(16);
        RectF a, b;
        RowRects(r, &a, &b);
        BOOL hot = (i == g_rowHot);
        if (r->type == R_HDR) {
            DrawTextBox(g, r->label, g_fontBold, g_x0 + 4, r->y + Sc(10), g_cw, Sc(24), cSub, TRUE);
            continue;
        }
        /* separator line between rows of a card */
        if (i > 0 && g_row[i - 1].page == g_page && g_row[i - 1].type != R_HDR) {
            GpPen *p;
            GdipCreatePen1(cLine, 1, 2, &p);
            GdipDrawLine(g, p, xl, r->y, xr, r->y);
            GdipDeletePen(p);
        }
        if (hot && (r->type == R_BTN || r->type == R_ITEM)) FillRR(g, cHover, g_x0 + 3, r->y + 2, g_cw - 6, r->h - 4, 10);
        float ly = r->y + (r->label2 && r->type == R_TOGGLE ? Sc(7) : 0);
        float lh = (r->label2 && r->type == R_TOGGLE) ? (float)Sc(24) : r->h;
        switch (r->type) {
        case R_TOGGLE: {
            BOOL on = r->v ? *r->v : GetSpecial(r->act);
            DrawTextBox(g, r->label, g_font, xl, ly, g_cw * 0.7f, lh, cText, TRUE);
            if (r->label2) DrawTextBox(g, r->label2, g_font, xl, ly + Sc(22), g_cw * 0.8f, Sc(22), cSub, TRUE);
            DrawToggle(g, &a, on);
            break;
        }
        case R_SLIDER: {
            DrawTextBox(g, r->label, g_font, xl, r->y, g_cw * 0.36f, r->h, cText, TRUE);
            float t = (float)(*r->v - r->mn) / (r->mx - r->mn);
            float cy = r->y + r->h / 2;
            FillRR(g, cTrack, a.X, cy - 2.5f, a.Width, 5, 2.5f);
            FillRR(g, ARGBC(255, 10, 132, 255), a.X, cy - 2.5f, a.Width * t, 5, 2.5f);
            float kx = a.X + a.Width * t;
            FillRR(g, ARGBC(60, 0, 0, 0), kx - 10, cy - 8, 20, 20, 10);
            FillRR(g, ARGBC(255, 255, 255, 255), kx - 9, cy - 9, 18, 18, 9);
            StrokeRR(g, ARGBC(70, 120, 120, 130), 1, kx - 9, cy - 9, 18, 18, 9);
            WCHAR v[32];
            wsprintfW(v, L"%d%s", *r->v, r->suffix ? r->suffix : L"");
            FillRR(g, g_uiDark ? ARGBC(255, 55, 57, 66) : ARGBC(255, 238, 240, 246), b.X, b.Y, b.Width, b.Height, 8);
            DrawCentered(g, v, g_font, b.X, b.Y, b.Width, b.Height, cText);
            break;
        }
        case R_COMBO: {
            DrawTextBox(g, r->label, g_font, xl, r->y, g_cw * 0.45f, r->h, cText, TRUE);
            int cur = *r->v < r->nopts ? *r->v : 0;
            FillRR(g, g_uiDark ? ARGBC(255, 55, 57, 66) : ARGBC(255, 238, 240, 246), a.X, a.Y, a.Width, a.Height, 9);
            DrawTextBox(g, r->opts[cur], g_font, a.X + 12, a.Y, a.Width - 36, a.Height, cText, TRUE);
            DrawChevronDown(g, a.X + a.Width - 16, a.Y + a.Height / 2, cSub);
            break;
        }
        case R_BTN:
            DrawTextBox(g, r->label, g_font, xl, r->y, g_cw * 0.8f, r->h, cText, TRUE);
            DrawChevronRight(g, xr - 6, r->y + r->h / 2, cSub);
            break;
        case R_BTN2:
            DrawTextBox(g, L"", g_font, xl, r->y, 10, r->h, cText, TRUE);
            FillRR(g, ARGBC(255, 10, 132, 255), a.X, a.Y, a.Width, a.Height, 9);
            DrawCentered(g, r->label, g_font, a.X, a.Y, a.Width, a.Height, ARGBC(255, 255, 255, 255));
            FillRR(g, g_uiDark ? ARGBC(255, 62, 64, 74) : ARGBC(255, 232, 235, 243), b.X, b.Y, b.Width, b.Height, 9);
            DrawCentered(g, r->label2, g_font, b.X, b.Y, b.Width, b.Height, cText);
            break;
        case R_COLOR: {
            DrawTextBox(g, r->label, g_font, xl, r->y, g_cw * 0.5f, r->h, cText, TRUE);
            int c = *r->v;
            DrawCentered(g, c >= 0 ? L"重置" : L"自动", g_font, b.X, b.Y, b.Width, b.Height, cSub);
            FillRR(g, c >= 0 ? ARGBC(255, GetRValue(c), GetGValue(c), GetBValue(c)) : cTrack, a.X, a.Y, a.Width, a.Height, 9);
            StrokeRR(g, cLine, 1, a.X, a.Y, a.Width, a.Height, 9);
            break;
        }
        case R_TEXT: {
            if (!lstrcmpW(r->label, L"ABOUT")) {
                float cx = g_x0 + g_cw / 2;
                HICON ic = LoadIconW(g_inst, MAKEINTRESOURCEW(1));
                DrawIconEx(dc, (int)(cx - Sc(36)), (int)(r->y + Sc(22)), ic, Sc(72), Sc(72), 0, NULL, DI_NORMAL);
                DrawCentered(g, L"MiniDock", g_fontTitle, g_x0, r->y + Sc(100), g_cw, Sc(36), cText);
                DrawCentered(g, L"版本 " APPVER L"  ·  64 位  ·  免安装", g_font, g_x0, r->y + Sc(138), g_cw, Sc(24), cSub);
                RectF tr = { g_x0 + Sc(40), r->y + Sc(176), g_cw - Sc(80), Sc(190) };
                GpBrush *br;
                GdipCreateSolidFill(cText, &br);
                GdipDrawString(g, L"一个极简、低内存占用的 Windows Dock 栏，风格参考 macOS：固定应用、文件夹、网页应用，应用组，磨砂玻璃，悬停放大与窗口预览，全部功能都可单独关闭以追求极致低占用。\n\n"
                                  L"作者：Wangcy\n使用 Claude Code 构建\n\nMade by Wangcy", -1, g_font, &tr, g_fmtW, br);
                GdipDeleteBrush(br);
            } else {
                RectF tr = { xl, r->y + Sc(6), g_cw - Sc(36), r->h - Sc(8) };
                GpBrush *br;
                GdipCreateSolidFill(cSub, &br);
                GdipDrawString(g, r->label, -1, g_font, &tr, g_fmtW, br);
                GdipDeleteBrush(br);
            }
            break;
        }
        case R_STAT: {
            PROCESS_MEMORY_COUNTERS_EX pm;
            ZeroMemory(&pm, sizeof pm);
            pm.cb = sizeof pm;
            GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS *)&pm, sizeof pm);
            WCHAR v[80];
            wsprintfW(v, L"%d.%d MB / %d.%d MB", (int)(pm.WorkingSetSize >> 20), (int)((pm.WorkingSetSize >> 10) % 1024 * 10 / 1024),
                      (int)(pm.PrivateUsage >> 20), (int)((pm.PrivateUsage >> 10) % 1024 * 10 / 1024));
            DrawTextBox(g, r->label, g_font, xl, r->y, g_cw * 0.5f, r->h, cText, TRUE);
            DrawTextBoxR(g, v, g_fontBold, xl, r->y, g_cw - Sc(36), r->h, cText);
            break;
        }
        case R_ITEM: {
            int ii = r->act - A_ITEM;
            DrawTextBox(g, r->label, g_font, xl, r->y, g_cw * 0.45f, r->h, cText, TRUE);
            const WCHAR *st = L"点击更换图标";
            ARGB sc = cSub;
            WCHAR tmp[MAX_PATH];
            if (ii >= 0 && ii < nItems && g_mstate[ii] == 1) { lstrcpynW(tmp, PathFindFileNameW(g_match[ii]), MAX_PATH); st = tmp; sc = ARGBC(255, 52, 199, 89); }
            else if (ii >= 0 && ii < nItems && g_mstate[ii] == 2) { st = L"未找到对应图标"; sc = ARGBC(255, 255, 69, 58); }
            else if (ii >= 0 && ii < nItems && g_mstate[ii] == 3) { st = L"已更换"; sc = ARGBC(255, 52, 199, 89); }
            DrawTextBoxR(g, st, g_font, xl, r->y, g_cw - Sc(52), r->h, sc);
            DrawChevronRight(g, xr - 6, r->y + r->h / 2, cSub);
            break;
        }
        }
    }
    /* top strip, sidebar, title, scrollbar are painted over the (clipped) rows */
    GpBrush *bb;
    GdipCreateSolidFill(cBg, &bb);
    GdipFillRectangle(g, bb, sbw, 0, W - sbw, (REAL)Sc(66));
    GdipDeleteBrush(bb);
    GdipCreateSolidFill(cSide, &bb);
    GdipFillRectangle(g, bb, 0, 0, sbw, (REAL)H);
    GdipDeleteBrush(bb);
    {
        GpPen *p;
        GdipCreatePen1(cLine, 1, 2, &p);
        GdipDrawLine(g, p, sbw, 0, sbw, (REAL)H);
        GdipDeletePen(p);
    }
    DrawTextBox(g, L"MiniDock", g_fontTitle, (float)Sc(24), (float)Sc(18), sbw - Sc(30), (float)Sc(44), cText, TRUE);
    for (int i = 0; i < NPAGE; i++) {
        float iy = (float)(Sc(78) + i * Sc(42)), ix = (float)Sc(12), iw = sbw - Sc(24), ih = (float)Sc(36);
        BOOL sel = (i == g_page);
        if (sel) FillRR(g, ARGBC(255, 10, 132, 255), ix, iy, iw, ih, 10);
        else if (i == g_sbHot) FillRR(g, g_uiDark ? ARGBC(255, 48, 50, 58) : ARGBC(255, 222, 226, 237), ix, iy, iw, ih, 10);
        DrawTextBox(g, g_pageName[i], g_font, ix + Sc(16), iy, iw - Sc(16), ih, sel ? ARGBC(255, 255, 255, 255) : cText, TRUE);
    }
    DrawTextBox(g, L"v" APPVER, g_font, (float)Sc(24), (float)(H - Sc(40)), sbw - Sc(30), (float)Sc(24), cSub, TRUE);
    DrawTextBox(g, g_pageName[g_page], g_fontTitle, g_x0, (float)Sc(18), g_cw, (float)Sc(44), cText, TRUE);
    if (g_contentH > g_viewH) {
        float th = g_viewH * g_viewH / g_contentH, ty = Sc(78) + (g_viewH - th) * g_scroll / (g_contentH - g_viewH + Sc(20));
        FillRR(g, g_uiDark ? ARGBC(120, 255, 255, 255) : ARGBC(110, 0, 0, 0), W - 9.f, ty, 4, th, 2);
    }
    GdipDeleteGraphics(g);
}


/* ---- interaction ---- */
static void DoNewShortcut(HWND parent);

static int RowHit(float x, float y, int *part) {
    for (int i = 0; i < g_nrow; i++) {
        Row *r = &g_row[i];
        if (r->page != g_page || r->type == R_HDR || r->type == R_TEXT || r->type == R_STAT) continue;
        if (y < Sc(66) || y < r->y || y >= r->y + r->h || x < g_x0 || x > g_x0 + g_cw) continue;
        RectF a, b;
        RowRects(r, &a, &b);
        *part = 0;
        if (r->type == R_SLIDER) { if (InRect(&b, x, y)) *part = 3; else if (x >= a.X - 10 && x <= a.X + a.Width + 10) *part = 2; }
        else if (r->type == R_TOGGLE) *part = (InRect(&a, x, y) || x > g_x0 + g_cw * 0.5f) ? 1 : 0;
        else if (r->type == R_BTN2) { if (InRect(&a, x, y)) *part = 5; else if (InRect(&b, x, y)) *part = 6; }
        else if (r->type == R_COLOR) { if (InRect(&a, x, y)) *part = 7; else if (InRect(&b, x, y)) *part = 8; }
        else *part = 1;
        return i;
    }
    return -1;
}

static void SetSlider(Row *r, float x) {
    RectF a, b;
    RowRects(r, &a, &b);
    float t = (x - a.X) / a.Width;
    if (t < 0) t = 0;
    if (t > 1) t = 1;
    *r->v = r->mn + (int)(t * (r->mx - r->mn) + 0.5f);
}

static void SettingsRedraw(void) { if (g_hset) InvalidateRect(g_hset, NULL, FALSE); }

static void DoAct(HWND h, int act) {
    switch (act) {
    case A_ADDURL: DoAddUrl(h); break;
    case A_ADDFILE: DoAddFile(h); break;
    case A_ADDFOLDER: DoAddFolder(h); break;
    case A_NEWSC: DoNewShortcut(h); break;
    case A_NEWGROUP: NewEmptyGroup(); UpdateGeometry(); Kick(); break;
    case A_ADDTRASH: AddTrash(); UpdateGeometry(); Kick(); break;
    case A_ADDSEP: DoAddSep(-1); break;
    case A_EXP0: ExportConfig(h, 0); break;
    case A_EXP1: ExportConfig(h, 1); break;
    case A_EXP2: ExportConfig(h, 2); break;
    case A_IMPORT: ImportConfig(h); BuildRows(); break;
    case A_RESET:
        if (MessageBoxW(h, L"确定恢复默认的外观与行为设置吗？（已固定的图标不会被删除）", L"MiniDock", MB_YESNO | MB_ICONQUESTION) == IDYES) { DefaultCfg(); ApplySettings(); BuildRows(); }
        break;
    case A_OPENDIR: ShellExecuteW(NULL, L"open", g_dir, NULL, NULL, SW_SHOWNORMAL); break;
    case A_LOWMODE:
        if (cfg.bgMode == 2) cfg.bgMode = 0;
        cfg.reflect = 0; cfg.clock = 0; cfg.preview = 0; cfg.showRunning = 0; cfg.dots = 0; cfg.minAnim = 0;
        ApplySettings();
        break;
    case A_RECMODE:
        cfg.bgMode = 2; cfg.reflect = 0; cfg.clock = 0; cfg.preview = 1; cfg.showRunning = 1; cfg.dots = 1; cfg.minAnim = 1;
        ApplySettings();
        break;
    case A_PICKDIR: {
        BROWSEINFOW b;
        ZeroMemory(&b, sizeof b);
        b.hwndOwner = h;
        b.lpszTitle = L"选择存放图标的文件夹";
        b.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
        LPITEMIDLIST pi = SHBrowseForFolderW(&b);
        if (pi) { SHGetPathFromIDListW(pi, g_batchDir); CoTaskMemFree(pi); RunMatch(); BuildRows(); }
        break;
    }
    case A_AUTOMATCH: {
        if (!g_batchDir[0]) { MessageBoxW(h, L"请先选择图标所在的文件夹。", L"MiniDock", MB_OK | MB_ICONWARNING); break; }
        RunMatch();
        int ok = 0, miss = 0;
        WCHAR list[600] = L"";
        for (int i = 0; i < nItems; i++) {
            if (items[i].type == T_SEP || items[i].temp) continue;
            if (g_mstate[i] == 1 && ApplyIconFile(i, g_match[i])) { g_mstate[i] = 3; ok++; }
            else if (g_mstate[i] == 2) { miss++; if (miss <= 8) { lstrcatW(list, L"\n · "); lstrcatW(list, items[i].name); } }
        }
        ScheduleSave();
        Kick();
        BuildRows();
        WCHAR msg[900];
        if (miss) wsprintfW(msg, L"已更换 %d 个图标。\n\n有 %d 个应用在该文件夹中没有找到对应图标：%s%s", ok, miss, list, miss > 8 ? L"\n · …" : L"");
        else wsprintfW(msg, L"已更换 %d 个图标。", ok);
        MessageBoxW(h, msg, L"批量设置图标", MB_OK | (miss ? MB_ICONWARNING : MB_ICONINFORMATION));
        break;
    }
    default:
        if (act >= A_ITEM) {
            int ii = act - A_ITEM;
            if (ii >= 0 && ii < nItems) {
                WCHAR f[MAX_PATH] = L"";
                OPENFILENAMEW o;
                ZeroMemory(&o, sizeof o);
                o.lStructSize = sizeof o; o.hwndOwner = h;
                o.lpstrFilter = L"图片 / 图标\0*.png;*.ico;*.jpg;*.jpeg;*.bmp;*.gif\0所有文件\0*.*\0";
                o.lpstrFile = f; o.nMaxFile = MAX_PATH; o.Flags = OFN_FILEMUSTEXIST;
                if (GetOpenFileNameW(&o) && ApplyIconFile(ii, f)) { g_mstate[ii] = 3; ScheduleSave(); Kick(); }
            }
        }
        break;
    }
}

static void ClampScroll(void) {
    float mx = g_contentH - g_viewH + Sc(24);
    if (mx < 0) mx = 0;
    if (g_scroll > mx) g_scroll = (int)mx;
    if (g_scroll < 0) g_scroll = 0;
}

static void SwitchPage(int p) {
    g_page = p;
    g_scroll = 0;
    if (p == 7) RunMatch();
    BuildRows();
    KillTimer(g_hset, 1);
    if (p == 6) SetTimer(g_hset, 1, 1000, NULL);
    SettingsRedraw();
}

static void DarkTitle(HWND h) {
    BOOL d = g_uiDark;
    DwmSetWindowAttribute(h, 20, &d, sizeof d);
}

static LRESULT CALLBACK SetProc(HWND h, UINT m, WPARAM w, LPARAM l) {
    switch (m) {
    case WM_ERASEBKGND: return 1;
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC dc = BeginPaint(h, &ps);
        RECT rc;
        GetClientRect(h, &rc);
        if (!g_sbmp || g_sw != rc.right || g_sh != rc.bottom) {
            if (g_sbmp) { SelectObject(g_sdc, g_sold); DeleteObject(g_sbmp); }
            g_sbmp = CreateCompatibleBitmap(dc, rc.right, rc.bottom);
            g_sold = (HBITMAP)SelectObject(g_sdc, g_sbmp);
            g_sw = rc.right; g_sh = rc.bottom;
        }
        PaintSettings(g_sdc, rc.right, rc.bottom);
        ClampScroll();
        BitBlt(dc, 0, 0, rc.right, rc.bottom, g_sdc, 0, 0, SRCCOPY);
        EndPaint(h, &ps);
        return 0;
    }
    case WM_TIMER: SettingsRedraw(); return 0;
    case WM_MOUSEMOVE: {
        float x = (float)GET_X_LPARAM(l), y = (float)GET_Y_LPARAM(l);
        if (g_rowDrag >= 0) {
            SetSlider(&g_row[g_rowDrag], x);
            UpdateGeometry();
            Kick();
            SettingsRedraw();
            return 0;
        }
        int sh = -1, part = 0, rh = -1;
        if (x < Sc(SBW)) { int i = (int)((y - Sc(78)) / Sc(42)); if (y >= Sc(78) && i >= 0 && i < NPAGE) sh = i; }
        else rh = RowHit(x, y, &part);
        if (sh != g_sbHot || rh != g_rowHot) {
            g_sbHot = sh; g_rowHot = rh;
            SettingsRedraw();
            TRACKMOUSEEVENT te = { sizeof te, TME_LEAVE, h, 0 };
            TrackMouseEvent(&te);
        }
        return 0;
    }
    case WM_MOUSELEAVE: g_sbHot = -1; g_rowHot = -1; SettingsRedraw(); return 0;
    case WM_MOUSEWHEEL:
        g_scroll -= (int)((short)HIWORD(w) / 120.0 * Sc(54));
        ClampScroll();
        SettingsRedraw();
        return 0;
    case WM_LBUTTONDOWN: {
        float x = (float)GET_X_LPARAM(l), y = (float)GET_Y_LPARAM(l);
        if (x < Sc(SBW)) {
            int i = (int)((y - Sc(78)) / Sc(42));
            if (y >= Sc(78) && i >= 0 && i < NPAGE) SwitchPage(i);
            return 0;
        }
        int part = 0, i = RowHit(x, y, &part);
        if (i < 0) return 0;
        Row *r = &g_row[i];
        RectF a, b;
        RowRects(r, &a, &b);
        if (r->type == R_TOGGLE && part == 1) {
            if (r->v) *r->v = !*r->v; else SetSpecial(r->act, !GetSpecial(r->act));
            ApplySettings();
            SettingsRedraw();
        } else if (r->type == R_SLIDER) {
            if (part == 2) { g_rowDrag = i; SetCapture(h); SetSlider(r, x); UpdateGeometry(); Kick(); SettingsRedraw(); }
            else if (part == 3) {
                WCHAR t[40], p[96];
                wsprintfW(t, L"%d", *r->v);
                wsprintfW(p, L"输入数值（%d – %d）：", r->mn, r->mx);
                if (InputBox(h, r->label, p, t, 40, NULL, NULL, 0)) {
                    int v = _wtoi(t);
                    if (v < r->mn) v = r->mn;
                    if (v > r->mx) v = r->mx;
                    *r->v = v;
                    ApplySettings();
                    SettingsRedraw();
                }
            }
        } else if (r->type == R_COMBO) {
            CmItem mi[12];
            int n = 0;
            for (int k = 0; k < r->nopts && k < 12; k++) n = CmAdd(mi, n, k + 1, r->opts[k], *r->v == k ? CM_CHECK : 0);
            POINT pt = { (int)(a.X + a.Width / 2), (int)(a.Y + a.Height + 4) };
            ClientToScreen(h, &pt);
            g_menuEdge = E_TOP;
            int res = ShowMenu(mi, n, pt.x, pt.y);
            g_menuEdge = -1;
            if (res > 0) { *r->v = res - 1; ApplySettings(); BuildRows(); }
            SettingsRedraw();
        } else if (r->type == R_BTN || (r->type == R_BTN2 && part == 5)) { DoAct(h, r->act); SettingsRedraw(); }
        else if (r->type == R_BTN2 && part == 6) { DoAct(h, r->act2); SettingsRedraw(); }
        else if (r->type == R_ITEM) { DoAct(h, r->act); BuildRows(); SettingsRedraw(); }
        else if (r->type == R_COLOR) {
            if (part == 7) {
                static COLORREF custom[16];
                CHOOSECOLORW cc;
                ZeroMemory(&cc, sizeof cc);
                cc.lStructSize = sizeof cc; cc.hwndOwner = h; cc.lpCustColors = custom;
                cc.rgbResult = *r->v >= 0 ? (COLORREF)*r->v : RGB(60, 120, 230);
                cc.Flags = CC_FULLOPEN | CC_RGBINIT;
                if (ChooseColorW(&cc)) { *r->v = (int)cc.rgbResult; cfg.accentTone = 0; ApplySettings(); }
            } else if (part == 8) { *r->v = -1; ApplySettings(); }
            SettingsRedraw();
        }
        return 0;
    }
    case WM_LBUTTONUP:
        if (g_rowDrag >= 0) { g_rowDrag = -1; ReleaseCapture(); ApplySettings(); SettingsRedraw(); }
        return 0;
    case WM_CLOSE: DestroyWindow(h); return 0;
    case WM_DESTROY:
        KillTimer(h, 1);
        if (g_sbmp) { SelectObject(g_sdc, g_sold); DeleteObject(g_sbmp); g_sbmp = NULL; }
        g_hset = NULL;
        SaveNow();
        return 0;
    }
    return DefWindowProcW(h, m, w, l);
}

static void ShowSettings(int page) {
    if (g_hset) { ShowWindow(g_hset, SW_SHOWNORMAL); SetForegroundWindow(g_hset); if (page >= 0) SwitchPage(page); return; }
    int W = Sc(880), H = Sc(660);
    RECT r = { 0, 0, W, H };
    AdjustWindowRect(&r, WS_CAPTION | WS_SYSMENU, FALSE);
    RECT wa = g_mi.rcWork;
    g_page = page < 0 ? 0 : page;
    g_scroll = 0;
    if (!g_sdc) { HDC d = GetDC(NULL); g_sdc = CreateCompatibleDC(d); ReleaseDC(NULL, d); }
    UpdateTheme();
    BuildRows();
    g_hset = CreateWindowExW(0, CLS_SET, L"MiniDock 设置", WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
                             (wa.left + wa.right - (r.right - r.left)) / 2, (wa.top + wa.bottom - (r.bottom - r.top)) / 3,
                             r.right - r.left, r.bottom - r.top, NULL, NULL, g_inst, NULL);
    if (!g_hset) return;
    DarkTitle(g_hset);
    ShowWindow(g_hset, SW_SHOWNORMAL);
    SetForegroundWindow(g_hset);
    if (g_page == 6) SetTimer(g_hset, 1, 1000, NULL);
}

/* ======================= tray ======================= */
static NOTIFYICONDATAW g_nid;
static void AddTray(void) {
    ZeroMemory(&g_nid, sizeof g_nid);
    g_nid.cbSize = sizeof g_nid;
    g_nid.hWnd = g_hwnd;
    g_nid.uID = 1;
    g_nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    g_nid.uCallbackMessage = WM_APP_TRAY;
    g_nid.hIcon = LoadIconW(g_inst, MAKEINTRESOURCEW(1));
    lstrcpyW(g_nid.szTip, L"MiniDock");
    Shell_NotifyIconW(NIM_ADD, &g_nid);
}

/* ======================= settings -> runtime ======================= */
static BOOL g_hkReg;
static void ApplySettings(void) {
    if (cfg.zoomSize < cfg.iconSize) cfg.zoomSize = cfg.iconSize;
    int need = CacheFor(cfg.zoom ? cfg.zoomSize : cfg.iconSize);
    if (need > g_cacheSize) { g_cacheSize = need; ReloadAllIcons(); }
    UpdateTheme();
    RefreshAccent();
    if (cfg.mode == 0) { gHideTgt = 0; KillTimer(g_hwnd, T_HIDE); }
    else { SetTimer(g_hwnd, T_HIDE, 120, NULL); gLastNear = GetTickCount(); }
    if (cfg.hotkey && !g_hkReg) g_hkReg = RegisterHotKey(g_hwnd, 1, MOD_CONTROL | MOD_ALT, 'D');
    else if (!cfg.hotkey && g_hkReg) { UnregisterHotKey(g_hwnd, 1); g_hkReg = FALSE; }
    SetMinHook(cfg.minAnim);
    if (cfg.notes) AddNote(); else RemoveNote();
    if (cfg.clock) SetTimer(g_hwnd, T_CLOCK, 30000, NULL); else KillTimer(g_hwnd, T_CLOCK);
    ClosePreview();
    UpdateGeometry();
    if (cfg.bgMode == 2) UpdateFrost();
    Kick();
    ScheduleSave();
    if (g_hset) { DarkTitle(g_hset); InvalidateRect(g_hset, NULL, FALSE); }
}

/* ======================= dock window ======================= */
static void EndDrag(BOOL commit) {
    int di = dragIdx, mt = mergeT;
    BOOL rem = removing;
    dragging = FALSE; removing = FALSE; dragIdx = -1; mergeT = -1;
    if (commit && di >= 0 && di < nItems) {
        if (rem) { if (!items[di].temp) RemoveItem(di); }
        else if (mt >= 0 && mt < nItems) MergeInto(di, mt);
        else ScheduleSave();
        UpdateGeometry();
    }
    Kick();
}

static BOOL CursorNearDock(POINT p) {
    POINT c = p;
    ScreenToClient(g_hwnd, &c);
    float u, v;
    ToLogical((float)c.x, (float)c.y, &u, &v);
    return u >= barL - 10 && u <= barL + barW + 10 && v >= barTop - (cfg.zoom ? M.Z : M.S) - 12 && v <= LH + 4;
}

static void OnAutoHide(void) {
    if (cfg.mode == 0 || gForceHide) return;
    POINT p;
    GetCursorPos(&p);
    RECT mr = g_mi.rcMonitor;
    BOOL edgeHit = FALSE;
    switch (cfg.edge) {
    case E_TOP: edgeHit = p.y <= mr.top + 1 && p.x >= winX && p.x < winX + winW; break;
    case E_LEFT: edgeHit = p.x <= mr.left + 1 && p.y >= winY && p.y < winY + winH; break;
    case E_RIGHT: edgeHit = p.x >= mr.right - 2 && p.y >= winY && p.y < winY + winH; break;
    default: edgeHit = p.y >= mr.bottom - 2 && p.x >= winX && p.x < winX + winW; break;
    }
    BOOL near_ = gHideTgt ? edgeHit : CursorNearDock(p);
    DWORD now = GetTickCount();
    if (near_ || gMenuOpen || dragging || gapIdx >= 0 || g_pv) {
        gLastNear = now;
        if (gHideTgt) { gHideTgt = 0; Kick(); }
        return;
    }
    if (cfg.mode == 2) {
        OvCtx oc;
        float bx, by, bw, bh;
        MapR(barL, barTop, barW, M.barH + M.margin, &bx, &by, &bw, &bh);
        oc.bar.left = winX + (int)bx; oc.bar.top = winY + (int)by;
        oc.bar.right = oc.bar.left + (int)bw; oc.bar.bottom = oc.bar.top + (int)bh;
        oc.hit = FALSE;
        EnumWindows(OvProc, (LPARAM)&oc);
        if (!oc.hit) { if (gHideTgt) { gHideTgt = 0; Kick(); } gLastNear = now; return; }
    }
    if (!gHideTgt && now - gLastNear > 450) { gHideTgt = 1; hover = FALSE; mouseU = -1; Kick(); }
}

static DWORD g_hotTick;
static int g_lastHot = -2;

static int g_mergeCand = -1;
static DWORD g_mergeTick;

static void OnHoverTimer(HWND h) {
    POINT p;
    GetCursorPos(&p);
    if (dragging && g_mergeCand >= 0 && mergeT != g_mergeCand && GetTickCount() - g_mergeTick > 220) { mergeT = g_mergeCand; Kick(); }
    if (!gMenuOpen && !dragging) {
        POINT c = p;
        ScreenToClient(h, &c);
        float u, v;
        ToLogical((float)c.x, (float)c.y, &u, &v);
        BOOL inZone = gHideCur < 0.5f && u >= barL - 4 && u <= barL + barW + 4 && v >= barTop - (cfg.zoom ? M.Z : M.S) - 14 && v <= LH && c.x >= -2 && c.y >= -2 && c.x <= winW + 2 && c.y <= winH + 2;
        if (inZone) {
            hover = TRUE;
            if (u != mouseU || v != mouseV) { mouseU = u; mouseV = v; Kick(); }
        } else if (hover) { hover = FALSE; mouseU = -1; mouseV = -1; hot = -1; Kick(); }
    }
    if (hot != g_lastHot) { g_lastHot = hot; g_hotTick = GetTickCount(); }
    if (g_pv) {
        RECT pr;
        GetWindowRect(g_pv, &pr);
        BOOL inPv = PtInRect(&pr, p);
        if (!inPv && !(hover && hot == pvItem)) ClosePreview();
    } else if (cfg.preview && hover && hot >= 0 && !gMenuOpen && !dragging && hot < nItems && items[hot].target[0] && items[hot].running && GetTickCount() - g_hotTick > 450) {
        ShowPreview(hot);
    }
    if (!hover && !g_pv) KillTimer(h, T_HOVER);
}

static LRESULT CALLBACK DockProc(HWND h, UINT m, WPARAM w, LPARAM l) {
    switch (m) {
    case WM_MOUSEACTIVATE: return MA_NOACTIVATE;
    case WM_ERASEBKGND: return 1;
    case WM_PAINT: { PAINTSTRUCT ps; BeginPaint(h, &ps); EndPaint(h, &ps); return 0; }
    case WM_MOUSEMOVE: {
        int x = GET_X_LPARAM(l), y = GET_Y_LPARAM(l);
        float u, v;
        ToLogical((float)x, (float)y, &u, &v);
        if (!gMenuOpen) { mouseU = u; mouseV = v; }
        if (!hover && !gMenuOpen) {
            hover = TRUE;
            SetTimer(h, T_HOVER, 40, NULL);
            TRACKMOUSEEVENT te = { sizeof te, TME_LEAVE, h, 0 };
            TrackMouseEvent(&te);
        }
        if (GetCapture() == h && pressIdx >= 0) {
            if (!dragging && !cfg.lock && (abs(x - pressPt.x) > 6 || abs(y - pressPt.y) > 6) && !(items[pressIdx].temp && items[pressIdx].type == T_SEP)) {
                dragging = TRUE; dragIdx = pressIdx; ClosePreview();
            }
            if (dragging) {
                dragPtA.x = x; dragPtA.y = y;
                removing = (v < barTop - M.S * 1.1f);
                mergeT = -1;
                if (!removing) {
                    Item *d = &items[dragIdx];
                    int cand = -1;
                    for (int j = 0; j < nItems && cand < 0; j++) {
                        Item *t = &items[j];
                        if (j == dragIdx || t->type == T_SEP || d->type == T_SEP || d->type == T_GROUP) continue;
                        if (t->cur > M.S * 0.5f && fabsf(u - actC[j]) < t->cur * 0.40f) cand = j;
                    }
                    if (cand != g_mergeCand) { g_mergeCand = cand; g_mergeTick = GetTickCount(); }
                    if (cand >= 0) { if (GetTickCount() - g_mergeTick > 220) mergeT = cand; }   /* hover over an icon: hold, then merge into a group */
                    else if (!(d->temp && d->type == T_SEP)) {
                        int ni = 0, pc = PinnedCount();
                        for (int i = 0; i < nItems; i++) if (i != dragIdx && actC[i] < u) ni++;
                        BOOL pin = FALSE;
                        if (d->temp) { if (ni <= pc) pin = TRUE; }
                        else if (pc < nItems && ni > pc - 1) ni = pc - 1;
                        if (ni != dragIdx) {
                            MoveItem(dragIdx, ni);
                            dragIdx = ni; pressIdx = ni;
                            if (pin) { items[ni].temp = 0; SetTimer(h, T_SCAN, 300, NULL); }
                            RenderFrame();
                        } else if (pin) { items[ni].temp = 0; SetTimer(h, T_SCAN, 300, NULL); }
                    }
                }
            }
        }
        Kick();
        return 0;
    }
    case WM_MOUSELEAVE:
        if (!dragging && !gMenuOpen) { hover = FALSE; mouseU = -1; hot = -1; Kick(); }
        return 0;
    case WM_LBUTTONDOWN: {
        float u, v;
        ToLogical((float)GET_X_LPARAM(l), (float)GET_Y_LPARAM(l), &u, &v);
        pressIdx = HitIndex(u, v);
        pressPt.x = GET_X_LPARAM(l); pressPt.y = GET_Y_LPARAM(l);
        if (pressIdx >= 0) SetCapture(h);
        return 0;
    }
    case WM_LBUTTONUP: {
        BOOL was = dragging;
        int pi = pressIdx;
        pressIdx = -1;
        if (GetCapture() == h) ReleaseCapture();
        if (was) {
            EndDrag(TRUE);
            POINT cp;
            GetCursorPos(&cp);
            ScreenToClient(h, &cp);
            float u, v;
            ToLogical((float)cp.x, (float)cp.y, &u, &v);
            if (u < barL || u > barL + barW || v < barTop - M.Z - 16 || v > LH) { hover = FALSE; mouseU = -1; hot = -1; Kick(); }
        } else if (pi >= 0) {
            float u, v;
            ToLogical((float)GET_X_LPARAM(l), (float)GET_Y_LPARAM(l), &u, &v);
            if (HitIndex(u, v) == pi) { ClosePreview(); LaunchItem(pi, GetKeyState(VK_SHIFT) < 0); }
        }
        return 0;
    }
    case WM_MBUTTONUP: {
        float u, v;
        ToLogical((float)GET_X_LPARAM(l), (float)GET_Y_LPARAM(l), &u, &v);
        int hi = HitIndex(u, v);
        if (hi >= 0) LaunchItem(hi, TRUE);
        return 0;
    }
    case WM_CAPTURECHANGED:
        if (dragging) EndDrag(FALSE);
        return 0;
    case WM_RBUTTONUP: {
        float u, v;
        ToLogical((float)GET_X_LPARAM(l), (float)GET_Y_LPARAM(l), &u, &v);
        ShowMenuAt(HitIndex(u, v), FALSE);
        return 0;
    }
    case WM_TIMER:
        if (w == T_ANIM) {
            if (!RenderFrame()) { KillTimer(h, T_ANIM); gAnimOn = FALSE; SetTimer(h, T_TRIM, 2500, NULL); }
        } else if (w == T_SCAN) {
            KillTimer(h, T_SCAN);
            SetTimer(h, T_SCAN, 2500, NULL);
            BOOL fs = IsFullscreenForeground();
            if (fs && !gFsHidden) { gFsHidden = TRUE; ShowWindow(h, SW_HIDE); }
            else if (!fs && gFsHidden) { gFsHidden = FALSE; ShowWindow(h, SW_SHOWNA); Kick(); }
            if (!fs) {
                ScanRunning();
                if (!gMenuOpen) SetWindowPos(h, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
            }
        } else if (w == T_HIDE) OnAutoHide();
        else if (w == T_HOVER) OnHoverTimer(h);
        else if (w == T_SAVE) { KillTimer(h, T_SAVE); SaveNow(); }
        else if (w == T_MIN) AnimStep();
        else if (w == T_CLOCK) Kick();
        else if (w == T_TRIM) { KillTimer(h, T_TRIM); SetProcessWorkingSetSize(GetCurrentProcess(), (SIZE_T)-1, (SIZE_T)-1); }
        return 0;
    case WM_HOTKEY: ToggleHide(); return 0;
    case WM_DISPLAYCHANGE:
    case WM_SETTINGCHANGE:
        UpdateGeometry();
        Kick();
        return 0;
    case WM_APP_FAIL:
        MessageBoxW(g_hset, L"无法识别这个拖入的项目（例如无法获取网址的网页应用快捷方式）。\n\n请改用右键 →“新建快捷方式”，输入应用路径或网址来添加。", L"MiniDock", MB_OK | MB_ICONINFORMATION);
        return 0;
    case WM_APP_AB:
        if (w == ABN_POSCHANGED) { g_abKey = 0; UpdateGeometry(); Kick(); }
        return 0;
    case WM_APP_ICON:
        g_noDl = TRUE;
        for (int i = 0; i < nItems; i++) {
            if (!items[i].fav && (items[i].type == T_URL || items[i].icon_src[0])) LoadItemIcon(&items[i]);
            for (int k = 0; k < items[i].nk; k++)
                if (!items[i].kids[k].fav && (items[i].kids[k].type == T_URL || items[i].kids[k].icon_src[0])) LoadItemIcon(&items[i].kids[k]);
            if (items[i].type == T_GROUP) BuildGroupIcon(&items[i]);
        }
        g_noDl = FALSE;
        Kick();
        return 0;
    case WM_APP_TRAY:
        if (LOWORD(l) == WM_RBUTTONUP || LOWORD(l) == WM_CONTEXTMENU) { g_menuEdge = E_BOTTOM; ShowMenuAt(-1, TRUE); g_menuEdge = -1; }
        else if (LOWORD(l) == WM_LBUTTONDBLCLK) ShowSettings(0);
        return 0;
    case WM_DESTROY:
        SaveNow();
        SetMinHook(FALSE);
        SetupAppBar(FALSE, 0);
        if (g_hkReg) UnregisterHotKey(h, 1);
        ClosePreview();
        Shell_NotifyIconW(NIM_DELETE, &g_nid);
        RevokeDragDrop(h);
        PostQuitMessage(0);
        return 0;
    }
    if (m == g_taskbarMsg && m) { AddTray(); return 0; }
    return DefWindowProcW(h, m, w, l);
}

/* ======================= entry ======================= */
int WINAPI WinMain(HINSTANCE hi, HINSTANCE hp, LPSTR cmd, int show) {
    g_inst = hi;
    HANDLE mtx = CreateMutexW(NULL, TRUE, L"Local\\MiniDock_Wangcy_Singleton");
    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        HWND ex = FindWindowW(CLS_DOCK, NULL);
        if (ex) PostMessageW(ex, WM_APP_TRAY, 0, WM_LBUTTONDBLCLK);
        return 0;
    }
    HDC sdc = GetDC(NULL);
    g_dpi = GetDeviceCaps(sdc, LOGPIXELSX);
    ReleaseDC(NULL, sdc);
    OleInitialize(NULL);
    INITCOMMONCONTROLSEX ic = { sizeof ic, ICC_BAR_CLASSES };
    InitCommonControlsEx(&ic);
    GdiplusStartupInput gi = { 1, NULL, FALSE, FALSE };
    GdiplusStartup(&g_gdipTok, &gi, NULL);

    InitPaths();
    LoadConfig();
    g_cacheSize = CacheFor(cfg.zoom ? cfg.zoomSize : cfg.iconSize);

    GdipCreateImageAttributes(&g_attr);
    GdipSetImageAttributesWrapMode(g_attr, 3, 0, FALSE);
    GdipCreateImageAttributes(&g_attrFade);
    GdipSetImageAttributesWrapMode(g_attrFade, 3, 0, FALSE);
    {
        float cm[25] = { 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0.78f, 0, 0, 0, 0, 0, 1 };
        GdipSetImageAttributesColorMatrix(g_attrFade, 1, TRUE, cm, NULL, 0);
    }
    GdipCreateImageAttributes(&g_attrFrost);
    GdipSetImageAttributesWrapMode(g_attrFrost, 3, 0, FALSE);
    GdipCreateImageAttributes(&g_attrRefl);
    GdipSetImageAttributesWrapMode(g_attrRefl, 3, 0, FALSE);
    GdipCreateImageAttributes(&g_attrRefl2);
    GdipSetImageAttributesWrapMode(g_attrRefl2, 3, 0, FALSE);
    {
        float c1[25] = { 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0.30f, 0, 0, 0, 0, 0, 1 };
        float c2[25] = { 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0.11f, 0, 0, 0, 0, 0, 1 };
        GdipSetImageAttributesColorMatrix(g_attrRefl, 1, TRUE, c1, NULL, 0);
        GdipSetImageAttributesColorMatrix(g_attrRefl2, 1, TRUE, c2, NULL, 0);
    }
    GpFontFamily *fam = NULL;
    if (GdipCreateFontFamilyFromName(L"Microsoft YaHei UI", NULL, &fam) != 0) GdipGetGenericFontFamilySansSerif(&fam);
    GdipCreateFont(fam, (REAL)Sc(13), 0, 2, &g_font);
    GdipCreateFont(fam, (REAL)Sc(15), 1, 2, &g_fontBold);
    GdipCreateFont(fam, (REAL)Sc(24), 1, 2, &g_fontTitle);
    GdipCreateStringFormat(0, 0, &g_fmtR);
    GdipSetStringFormatAlign(g_fmtR, 2);
    GdipSetStringFormatLineAlign(g_fmtR, 1);
    GdipSetStringFormatTrimming(g_fmtR, 3);
    GdipCreateStringFormat(0, 0, &g_fmtW);
    GdipCreateStringFormat(0, 0, &g_fmtC);
    GdipSetStringFormatAlign(g_fmtC, 1);
    GdipSetStringFormatLineAlign(g_fmtC, 1);
    GdipSetStringFormatTrimming(g_fmtC, 3);
    GdipCreateStringFormat(0x1000, 0, &g_fmtL);
    GdipSetStringFormatLineAlign(g_fmtL, 1);

    NONCLIENTMETRICSW ncm = { sizeof ncm };
    SystemParametersInfoW(SPI_GETNONCLIENTMETRICS, sizeof ncm, &ncm, 0);
    ncm.lfMessageFont.lfHeight = -Sc(14);
    lstrcpyW(ncm.lfMessageFont.lfFaceName, L"Microsoft YaHei UI");
    g_uiFont = CreateFontIndirectW(&ncm.lfMessageFont);
    ncm.lfMessageFont.lfHeight = -Sc(22);
    ncm.lfMessageFont.lfWeight = FW_SEMIBOLD;
    g_titleFont = CreateFontIndirectW(&ncm.lfMessageFont);

    cfUrlW = RegisterClipboardFormatW(L"UniformResourceLocatorW");
    cfUrlA = RegisterClipboardFormatW(L"UniformResourceLocator");
    cfShellIDList = RegisterClipboardFormatW(CFSTR_SHELLIDLIST);
    g_taskbarMsg = RegisterWindowMessageW(L"TaskbarCreated");

    WNDCLASSW wc;
    ZeroMemory(&wc, sizeof wc);
    wc.hInstance = hi;
    wc.hCursor = LoadCursorW(NULL, IDC_ARROW);
    wc.hIcon = LoadIconW(hi, MAKEINTRESOURCEW(1));
    wc.lpfnWndProc = DockProc; wc.lpszClassName = CLS_DOCK; RegisterClassW(&wc);
    wc.lpfnWndProc = SetProc; wc.lpszClassName = CLS_SET; wc.hbrBackground = NULL; RegisterClassW(&wc);
    wc.lpfnWndProc = InProc; wc.lpszClassName = CLS_IN; wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1); RegisterClassW(&wc);
    wc.hbrBackground = NULL;
    wc.lpfnWndProc = MenuProc; wc.lpszClassName = CLS_MENU; RegisterClassW(&wc);
    wc.lpfnWndProc = GrpProc; wc.lpszClassName = CLS_GRP; RegisterClassW(&wc);
    wc.lpfnWndProc = PvProc; wc.lpszClassName = CLS_PV; RegisterClassW(&wc);
    wc.lpfnWndProc = FanProc; wc.lpszClassName = CLS_FAN; RegisterClassW(&wc);
    wc.lpfnWndProc = NoteProc; wc.lpszClassName = CLS_NOTE; wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1); RegisterClassW(&wc);
    wc.lpfnWndProc = IcoProc; wc.lpszClassName = CLS_ICOD; RegisterClassW(&wc);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpfnWndProc = DefWindowProcW; wc.lpszClassName = CLS_ANIM; RegisterClassW(&wc);

    HDC dc = GetDC(NULL);
    g_memdc = CreateCompatibleDC(dc);
    g_fdc = CreateCompatibleDC(dc);
    ReleaseDC(NULL, dc);

    GetMon();
    LoadItems();
    for (int i = 0; i < nItems; i++) items[i].cur = 0;
    UpdateGeometry();

    g_hwnd = CreateWindowExW(WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE, CLS_DOCK, L"MiniDock", WS_POPUP,
                             winX, winY, winW, winH, NULL, NULL, hi, NULL);
    RegisterDragDrop(g_hwnd, &g_dt);
    AddTray();
    if (GetAutostart()) SetAutostart(TRUE);
    UpdateGeometry();
    ApplySettings();
    ShowWindow(g_hwnd, SW_SHOWNA);
    UpdateFrost();
    RenderFrame();
    Kick();
    SetTimer(g_hwnd, T_SCAN, 1500, NULL);

    MSG msg;
    while (GetMessageW(&msg, NULL, 0, 0) > 0) { TranslateMessage(&msg); DispatchMessageW(&msg); }
    for (int i = 0; i < nItems; i++) FreeItem(&items[i]);
    FreeSurface();
    GdiplusShutdown(g_gdipTok);
    OleUninitialize();
    if (mtx) CloseHandle(mtx);
    return 0;
}

