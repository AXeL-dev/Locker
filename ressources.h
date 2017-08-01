//=============================================================================
// Projet : Locker
// Fichier : ressources.h
//
//=============================================================================

#include <windows.h> // -lwinmm (Pour PlaySound();)


//=============================================================================
//                               Constantes
//=============================================================================

#define NOM_APP                  "Locker"

#define APP_VERSION              "v0.2"

#define MOT_DE_PASSE             "AXeL"

#define NOM_DOSSIER_SECRET       "#LockerSecret#123456789#"

#define IDI_ICONE1               100
#define IDI_ICONE2               101
#define IDI_ICONE3               102
#define IDI_ICONE4               103
#define IDI_ICONE5               104

/* DialogBox de mot de passe */
#define IDE_PASSWORD             105

/* DialogBox de configuration */
#define RB_UNLOCK_DOSSIER        106
#define RB_LOCK_DOSSIER          107
#define BT_APROPOS               108
#define BT_QUITTER               109

//=============================================================================
//                            Variables globales
//=============================================================================

HINSTANCE hInst;

BOOL UNLOCK_DOSSIER;

//=============================================================================
//                                Prototypes
//=============================================================================
BOOL APIENTRY MdpDlgProc(HWND hMdp, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL APIENTRY WmdpDlgProc(HWND hWmdp, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL APIENTRY ConfigDlgProc(HWND hCfg, UINT uMsg, WPARAM wParam, LPARAM lParam);
void goLocker(HWND hwnd, BOOL unlock);
