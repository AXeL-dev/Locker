//=============================================================================
// Projet : Locker
// Version : 0.2
// Fichier : main.c
// Auteur : AXeL
// Date de création : 09/11/2013
// Date de la dernière modification : 09/11/2013
// Lacunes : Une simple lecture de l'exé avec un éditeur de texte (notepad++) 
//           permettra de savoir le mot de passe et le nom du dossier
// Idées d'amélioration : - Pouvoir changer le mot de passe et le nom du dossier
//                          => fichier de configuration + gestion d'erreurs de ce
//                          dernier
// Mise à jour :
//                - 24/01/2014:
//                   - Réglage d'un beug critique au niveau de la fonction goLocker()
//                   - Modification de l'ancien mot de passe "1234"
//
//=============================================================================

#include "ressources.h"


//=============================================================================
//                            Fonction principale
//=============================================================================

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrecedente, LPSTR lpCmdLine, int nCmdShow)
{
   hInst = hInstance;
   DialogBox(hInstance, "PASSWORD", NULL, (DLGPROC)MdpDlgProc);
return 0;    
}

//=============================================================================
//                        DialogBox de mot de passe
//=============================================================================

BOOL APIENTRY MdpDlgProc(HWND hMdp, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
   switch (uMsg)
   {
       case WM_INITDIALOG:
            /* Affichage de l'icone */
            SendMessage(hMdp, WM_SETICON, ICON_BIG, (LPARAM)LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICONE1)));
            break;
       case WM_COMMAND:
            switch (LOWORD(wParam))
            {
                case IDOK:
                     {
                     char mdp[32];
                     GetDlgItemText(hMdp, IDE_PASSWORD, mdp, 32);
                     if (!lstrcmp(mdp, MOT_DE_PASSE))
                     {
                        EndDialog(hMdp, 0);
                        DialogBox(hInst, "CONFIG", NULL, (DLGPROC)ConfigDlgProc);
                     }
                     else
                        DialogBox(hInst, "WRONGPASSWORD", hMdp, (DLGPROC)WmdpDlgProc);
                     break;
                     }
                case IDCANCEL:
                     EndDialog(hMdp, 0);
                     break;
            }
            break;
   }
   
return 0; 
}

//=============================================================================
//                   DialogBox de mot de passe incorrect
//=============================================================================

BOOL APIENTRY WmdpDlgProc(HWND hWmdp, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
   switch (uMsg)
   {
       case WM_INITDIALOG:
            /* Affichage de l'icone */
            SendMessage(hWmdp, WM_SETICON, ICON_BIG, (LPARAM)LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICONE1)));
            /* On joue un son d'éxclamation */
            PlaySound((LPCTSTR)SND_ALIAS_SYSTEMEXCLAMATION, NULL, SND_ALIAS_ID | SND_ASYNC);
            break;
       case WM_COMMAND:
            switch (LOWORD(wParam))
            {
                case IDOK:
                case IDCANCEL:
                     EndDialog(hWmdp, 0);
                     break;
            }
            break;
   }
   
return 0; 
}

//=============================================================================
//                        DialogBox de configuration
//=============================================================================

BOOL APIENTRY ConfigDlgProc(HWND hCfg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
   switch (uMsg)
   {
       case WM_INITDIALOG:
            /* Affichage de l'icone */
            SendMessage(hCfg, WM_SETICON, ICON_BIG, (LPARAM)LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICONE1)));
            /* Activation du RadioButton RB_UNLOCK_DOSSIER */
            CheckDlgButton(hCfg, RB_UNLOCK_DOSSIER, BST_CHECKED);
            UNLOCK_DOSSIER = TRUE;
            goLocker(hCfg, UNLOCK_DOSSIER);
            break;
       case WM_COMMAND:
            switch (LOWORD(wParam))
            {
                case RB_UNLOCK_DOSSIER:
                     if (!UNLOCK_DOSSIER)
                     {
                        UNLOCK_DOSSIER = TRUE;
                        goLocker(hCfg, UNLOCK_DOSSIER);
                     }
                     break;
                case RB_LOCK_DOSSIER:
                     if (UNLOCK_DOSSIER)
                     {
                        UNLOCK_DOSSIER = FALSE;
                        goLocker(hCfg, UNLOCK_DOSSIER);
                     }
                     break;
                case BT_APROPOS:
                     MessageBox(hCfg, NOM_APP 
                     " " APP_VERSION
                     "\nPar : AXeL"
                     "\nCompatibilité : Windows XP/7"
                     "\nLibre de droit et d'utilisations."
                     ,"A propos", MB_OK | MB_ICONINFORMATION);
                     break;
                case BT_QUITTER:
                case IDCANCEL:
                     if (UNLOCK_DOSSIER)
                     {
                        MessageBox(hCfg, "Veuillez vérouiller les fichiers !", NOM_APP, MB_OK | MB_ICONSTOP);
                        break;
                     }
                     EndDialog(hCfg, 0);
                     break;
            }
            break;
   }
   
return 0; 
}

//=============================================================================
//    Fonction qui s'occupe de vérouiller/déverouiller le dossier secret
//=============================================================================

void goLocker(HWND hwnd, BOOL unlock)
{
   char path[MAX_PATH];
   int lockResult;
   GetCurrentDirectory(MAX_PATH, path); /* Récupération du dossier actuel */
   lstrcat(path, "\\");
   lstrcat(path, NOM_DOSSIER_SECRET); /* on y ajoute le nom du dossier secret */
   
   if (unlock)
   {
      lockResult = SetFileAttributes(path, FILE_ATTRIBUTE_NORMAL); /* Déverouillage */
      if (!lockResult) /* Si échec => dossier introuvable */
      {
         MessageBox(hwnd, "   Vu que le dossier secret n'a pas été trouvé"
                          "\ndans le répertoire actuel, ce dernier sera crée !"
                          , NOM_APP, MB_OK | MB_ICONINFORMATION);
         CreateDirectory(path, NULL); /* Création du dossier */
         goLocker(hwnd, TRUE);
      }
      else
         ShellExecute(NULL, "open", path, NULL, NULL, SW_SHOWNORMAL); /* Ouverture/Affichage */
   }
   else
   {
      lockResult = SetFileAttributes(path, FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM); /* Vérouillage */
      if (!lockResult) /* Si échec => dossier introuvable */
      {
         MessageBox(hwnd, "  Le dossier secret n'a pas été trouvé ou vien"
                          "\nd'être déplacé ou effacé, ce dernier sera crée !"
                          , NOM_APP, MB_OK | MB_ICONINFORMATION);
         CreateDirectory(path, NULL); /* Création du dossier */
         goLocker(hwnd, FALSE);
      }
      else
      {
         HWND newHwnd;
         /* Récupération du HWND de la fenêtre du dossier secret */
         newHwnd = FindWindow(NULL, NOM_DOSSIER_SECRET);
         //CloseWindow(newHwnd); /* Minimize */
         if (newHwnd != NULL) /* Si la fenêtre existe */
            SendMessage(newHwnd, WM_CLOSE, 0, 0); /* Fermeture */
      }
   }
}
