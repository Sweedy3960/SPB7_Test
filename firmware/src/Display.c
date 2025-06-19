/*************************************************************************** */
/** 
 * @file:   Display.c
 * #        DISPLAY.c 
 * ## Module de gestion de l'affichage du TPA
 * 
 * @author  J. Jose Moreno
 * @date    16.04.2020
 * @version 1.0
 * 
 * ## Description
 * Fournit les fonctions necessaires pour afficher les donnees du TPA/VPA sur
 * l'ecran LCD du TPA. Display_Task effectue une mise a jour reguliere de 
 * l'affichage complet a partir d'une copie complete locale. Elle utilise l'API
 * OpenSource ugui.h. pour dessiner des formes et du texte sur l'ecran graphique.
 * 
 * @author  J. Jose Moreno
 * @date    11.08.2023
 * @version 2.0
 * 
 * ## Description
 * Generalisation pour utilisation dans projets ETML-ES.
 * Remplacement de D... par D...
 */
/* ************************************************************************** */

// *****************************************************************************
// Section: Included Files
// *****************************************************************************
#include "Display.h"
#include "appDisp.h"
#include "math.h"
#include "appTouch.h"
/* ************************************************************************** */
/* ************************************************************************** */
/** @name Section: File Scope or Global Data                                  */
/* ************************************************************************** */
/* ************************************************************************** */
///@{
static T_DISPLAY disp;
char signalNames[7][20] = {
    "Signal1",
    "Signal2",
    "Signal3",
    "Signal4",
    "Signal5",
    "Signal6",
    "Signal7"
};
///@}
//extern MODULE_SLOT_DATA slotData[7];
/* ************************************************************************** */
/* ************************************************************************** */
/** @name Section: Task Callback Routines                                     */
/* ************************************************************************** */
/* ************************************************************************** */
///@{
/* -------------------------------------------------------------------------- */

/**
 @brief     Comptage des temps pour Display. Cette routine doit etre appelee
 *          par l'ISR d'un timer a une cadence de 10 ms.
 * 
 */
void Display_TimerCallback(void) {
    disp.refreshDelayCount++;
    if (disp.refreshDelayCount >= DISPLAY_REFRESH_PERIOD) {
        disp.refreshTickEvent = true;
        disp.refreshDelayCount = 0;
    }
    //LED2Off();
}
/* -------------------------------------------------------------------------- */

/**
 * @brief   Fonction de Callback appelee par la lib ugui pour generer un pixel 
 *          pour l'affiche cible.      
 * @param   x   axe des x sur le LCD. La valeur est utilisable directement.
 * @param   y   axe des y sur le LCD, il est segmente en 'pages' de 8 bits (bytes).
 *              La position du bit est calculee pour la bonne page.
 * @param   c   couleur (noir ou blanc du pixel
 */
void DisplayPixelSetCallback(UG_S16 x, UG_S16 y, UG_COLOR c) {
    //this was the original code bu wee need to shift 90

    //	UG_S16 column = x;
    //	UG_S16 row = DISPLAY_HEIGHT - y - 1;
    //so 
    UG_S16 column = DISPLAY_WIDTH - y - 1;
    UG_S16 row = DISPLAY_HEIGHT - x - 1;
    /* toute autre couleur que noir sera consideree comme blanche */
    uint8_t page = row / DISPLAY_PAGES;
    uint8_t pageValue = 1 << (row % DISPLAY_PAGES);
    if (c == C_BLACK) {
        /* noir pour afficher un pixel, val = val OR pageValue */
        disp.screenCopy[page][column] |= pageValue;
    } else {
        /* blanc pour effacer un pixel, val = val AND /pageValue */
        disp.screenCopy[page][column] &= ~pageValue;
    }
} /* DisplayPixelSetCallback */

///@}
/* ************************************************************************** */
/* ************************************************************************** */
/** @name Section: Local Functions                                            */
/* ************************************************************************** */
/* ************************************************************************** */
///@{
/* -------------------------------------------------------------------------- */

/**
 * @brief   Efface la page LCD en fonce ou clair selon le mode utilise
 * 
 * @param   p           page to be cleared
 * @param   setToClear  false = dark ; true = clear
 * 
 * Texec = 245 us
 */
static void DisplayClearPage(uint8_t p, bool setToClear) {
    uint8_t c, color;

    if (setToClear) {
        color = 0;
    } else {
        color = 0xFF;
    }

    for (c = 0; c < DISPLAY_WIDTH; c++) {
        disp.screenCopy[p][c] = color;
    }
} /* DisplayClearPage */

/* -------------------------------------------------------------------------- */

/**
 * @brief   Displays the welcome screen
 * 
 * @param   setToDark   true = dark ; false = clear
 * 
 */
static void DisplayScreen_Welcome(bool setToDark) {
    char str_Welcome_1[] = "Welcome";
    char str_Welcome_2[] = "v1.0.0";
    char str_Welcome_3[] = "init..";

    if (setToDark) {
        UG_SetBackcolor(C_WHITE);
        UG_SetForecolor(C_BLACK);
    } else {
        UG_SetBackcolor(C_BLACK);
        UG_SetForecolor(C_WHITE);
    }

    UG_FontSetHSpace(0);

    /* Ecriture des chaines de caracteres */
    UG_FontSelect(&FONT_8X8);
    UG_PutString(0, 0, str_Welcome_1);
    UG_FontSelect(&FONT_6X8);
    UG_PutString(0, 30, str_Welcome_2);
    UG_FontSelect(&FONT_4X6);
    UG_PutString(0, 45, str_Welcome_3);

} /* DisplayScreen_Welcome */

void DisplayScreen_MainMenu(uint16_t * stateTouch, bool setToDark) {
    static uint8_t menuIndex = 0; // 0 = Afficher signaux, 1 = Renommer un signal, 2 = Test Buzzer
    char *menuItems[] = {
        "Afficher signaux",
        "Renommer un signal",
        "Test Buzzer"
    };
    uint8_t nbItems = sizeof (menuItems) / sizeof (menuItems[0]);
    uint8_t i = 0;
    // Fond noir ou blanc
    if (setToDark) {
        UG_SetBackcolor(C_WHITE);
        UG_SetForecolor(C_BLACK);
    } else {
        UG_SetBackcolor(C_BLACK);
        UG_SetForecolor(C_WHITE);
    }
    UG_FontSetHSpace(0);
    UG_FontSelect(&FONT_6X8);
    // Titre
    UG_PutString(1, 2, "Main Menu");
    // Affichage des choix de menu
    for (i = 0; i < nbItems; i++) {
        if (i == menuIndex) {
            UG_SetBackcolor(C_BLACK);
            UG_SetForecolor(C_WHITE);
            UG_FillFrame(8, 20 + i * 15, 120, 20 + i * 15 + 10, C_BLACK);
        } else {
            UG_SetBackcolor(C_WHITE);
            UG_SetForecolor(C_BLACK);
        }
        UG_PutString(10, (20 + (i * 15)), menuItems[i]);
    }
    // Remettre couleurs par défaut
    if (setToDark) {
        UG_SetBackcolor(C_WHITE);
        UG_SetForecolor(C_BLACK);
    } else {
        UG_SetBackcolor(C_BLACK);
        UG_SetForecolor(C_WHITE);
    }
    if (stateTouch == NULL)
        return;
    // Gestion des touches
    switch (*stateTouch) {
        case KEY_UP_R_MASK:
            if (menuIndex > 0) {
                menuIndex--;
            }
            break;
        case KEY_DOWN_R_MASK:
            if (menuIndex < nbItems - 1) {
                menuIndex++;
            }
            break;
        case KEY_MID_R_MASK:
            // Valider le choix
            switch (menuIndex) {
                case 0:
                    App_Display_ChangeScreen(DISP_SIGN, stateTouch, false);
                    break;
                case 1:
                    App_Display_ChangeScreen(DISP_CHANGE_SIGN_NAME, stateTouch, false);
                    break;
                case 2:
                    App_Display_ChangeScreen(DISP_SCR_ERROR, stateTouch, false);
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}

void DisplayScreen_Error(uint16_t * stateTouch, bool setToDark) {
    if (setToDark) {
        UG_SetBackcolor(C_WHITE);
        UG_SetForecolor(C_BLACK);
    } else {
        UG_SetBackcolor(C_BLACK);
        UG_SetForecolor(C_WHITE);
    }
    UG_FontSetHSpace(0);
    UG_FontSelect(&FONT_6X8);
    UG_PutString(1, 1, "rel");
    UG_PutString(1, 10, "or");
    UG_PutString(1, 20, "not handle");

}
/* -------------------------------------------------------------------------- */

/**
 * @brief   Displays Screen Voltmeter 23132
 * 
 * @param   setToDark   true = dark ; false = clear
 * 
 */
void DisplayScreen_23132(bool setToDark) {
    if (setToDark) {
        UG_SetBackcolor(C_WHITE);
        UG_SetForecolor(C_BLACK);
    } else {
        UG_SetBackcolor(C_BLACK);
        UG_SetForecolor(C_WHITE);
    }
    UG_FontSetHSpace(0);
    UG_FontSelect(&FONT_6X8);
    UG_PutString(1, 2, "Module 2");
    UG_DrawFrame(0, 0, 127, 63, C_BLACK);
    UG_DrawLine(0, 10, 127, 10, C_BLACK);
    UG_DrawLine(0, 53, 127, 53, C_BLACK);
    UG_DrawLine(32, 63, 32, 53, C_BLACK);
    UG_DrawLine(62, 63, 62, 53, C_BLACK);
    UG_DrawLine(77, 63, 77, 53, C_BLACK);
} /* DisplayScreen_23132 */

void DrawMenuIcon(bool selected) {
    int foreColor = C_BLACK;
    int backColor = C_WHITE;

    if (selected) {
        foreColor = C_WHITE;
        backColor = C_BLACK;
    }
    UG_FillFrame(1, 54, 31, 62, backColor);
    UG_DrawPixel(2, 56, foreColor);
    UG_DrawPixel(2, 58, foreColor);
    UG_DrawPixel(2, 60, foreColor);
    UG_DrawLine(4, 56, 6, 56, foreColor);
    UG_DrawLine(4, 58, 6, 58, foreColor);
    UG_DrawLine(4, 60, 6, 60, foreColor);
    UG_SetBackcolor(backColor);
    UG_SetForecolor(foreColor);
    UG_FontSelect(&FONT_6X8);
    UG_PutString(8, 55, "Menu");
}



/* -------------------------------------------------------------------------- */

/**
 * @brief   Dessine un logo "œil dans un triangle" centré sur l'écran LCD.
 *
 * @details Le logo est centré automatiquement selon DISPLAY_WIDTH et DISPLAY_HEIGHT.
 *          Les tailles sont adaptées pour un écran 128x64.
 */
void DisplayEyeLogo(void) {
    int centerX = DISPLAY_HEIGHT / 2;
    int centerY = DISPLAY_WIDTH / 2;

    // Triangle équilatéral
    int triHeight = 50;
    int triHalfBase = 32;
    int triTopY = centerY - triHeight / 2;
    int triBaseY = centerY + triHeight / 2;

    UG_DrawLine(centerX, triTopY, centerX - triHalfBase, triBaseY, C_BLACK);
    UG_DrawLine(centerX - triHalfBase, triBaseY, centerX + triHalfBase, triBaseY, C_BLACK);
    UG_DrawLine(centerX + triHalfBase, triBaseY, centerX, triTopY, C_BLACK);

    // Eye outline approximation (diamond shape instead of ellipse)
    int eyeWidth = 10;
    int eyeHeight = 30;
    DrawEllipse(centerX, centerY, eyeHeight, eyeWidth);
    // Pupille (cercle noir)
    UG_FillCircle(centerX, centerY, 5, C_BLACK);
    //7064464
    // Paupière supérieure (triangle noir)
    int lidOffsetY = 7;
    UG_DrawLine(centerX - eyeWidth / 2, centerY, centerX, centerY - lidOffsetY, C_BLACK);
    UG_DrawLine(centerX, centerY - lidOffsetY, centerX + eyeWidth / 2, centerY, C_BLACK);
    UG_DrawLine(centerX + eyeWidth / 2, centerY, centerX - eyeWidth / 2, centerY, C_BLACK);

    // Cadre optionnel
    UG_DrawFrame(centerX - 45, centerY - 28, centerX + 45, centerY + 28, C_GRAY);
}
///@}

/* ************************************************************************** */
/* ************************************************************************** */
/** @name Section: Interface Functions                                        */
/* ************************************************************************** */
/* ************************************************************************** */

/*
void DisplayValues_MainMenu(
    uint8_t positionList, uint8_t positionCursor,
    uint8_t moduleA, uint8_t moduleB, uint8_t moduleC)
{
    char slotA[20];
    char slotB[20];
    char slotC[20];


    if(disp.currentScreenNr == DISP_SCR_MAIN_MENU)
    {
        sprintf(slotA, "Slot %d: %s", (positionList+1), nameModulesData[moduleA]);
        sprintf(slotB, "Slot %d: %s", (positionList+2), nameModulesData[moduleB]);
        sprintf(slotC, "Slot %d: %s", (positionList+3), nameModulesData[moduleC]);

        UG_FontSelect(&FONT_6X8);
        UG_PutString(6, 16, slotA);
        UG_PutString(6, 33, slotB);
        UG_PutString(6, 50, slotC);
        UG_FillFrame(1, 11, 5, 62, C_WHITE);

        switch(positionCursor)
        {
            case 0:
                UG_DrawLine(2, 18, 4, 20, C_BLACK);	//TODO: Function for arrow with xy as parameters
                UG_DrawLine(2, 22, 4, 20, C_BLACK);
                break;
            case 1:
                UG_DrawLine(2, 35, 4, 37, C_BLACK);
                UG_DrawLine(2, 39, 4, 37, C_BLACK);
                break;
            case 2:
                UG_DrawLine(2, 52, 4, 54, C_BLACK);
                UG_DrawLine(2, 56, 4, 54, C_BLACK);
                break;
            default:
                break;
        }
    }
}
 */
///@{

/* -------------------------------------------------------------------------- */
/**
 * @brief   Sets the background color of the display
 * 
 * @param   rgb     standard RGB value. Ex: C_AQUA = 0x00FFFF
 * 
 */
/*
void DisplaySetBacklightRGB(uint32_t rgb){
    uint8_t col [3];
    memcpy(col, &rgb, 3);
    DRV_OC1_PulseWidthSet(col[2] * DISP_BLRGB_PWM_FACTOR);
    DRV_OC2_PulseWidthSet(col[1] * DISP_BLRGB_PWM_FACTOR);
    DRV_OC3_PulseWidthSet(col[0] * DISP_BLRGB_PWM_FACTOR);
}
 */
/* -------------------------------------------------------------------------- */

/**
 * @brief   Initiate a scroll clear of the display
 * 
 */
void DisplayScrollClear() {
    disp.command = DISP_CMD_SCROLLDOWN_CLEAR;
    disp.scrollLine = 0;
}

/* -------------------------------------------------------------------------- */

/**
 * @brief   Clear Display data
 *  
 * @param setToClear    false = dark ; true = clear 
 */
void DisplayClear(bool setToClear) {
    uint8_t p, c, color;

    if (setToClear) {
        color = 0;
    } else {
        color = 0xFF;
    }

    for (p = 0; p < DISPLAY_PAGES; p++) {
        for (c = 0; c < DISPLAY_WIDTH; c++) {
            disp.screenCopy[p][c] = color;
        }
    }
} /* DisplayClear */

/* -------------------------------------------------------------------------- */

/**
 * @brief   Display Screen basics
 * 
 * @param   screen      image # to be displayed
 * @param   setToDark   true = dark ; false = clear
 * 
 */
void DisplayScreen(uint8_t screen, uint16_t *touchStates, bool setToDark) {

    disp.currentScreenNr = screen;
    DisplayClear(true);
    uint16_t touchs = 0;
    if (touchStates != NULL)
    touchs = *touchStates;
    switch (screen) {
        case DISP_SCR_WELCOME:
            DisplayScreen_Welcome(setToDark);
            // UG_DrawLine(70, 55, 71, 57, 0);
            break;
        case DSP_LOGO:
            DisplayEyeLogo();
            break;
        case DISP_SIGN:
            DisplayScreen_Signals(&touchs, setToDark);
            break;
        case DISP_SCR_MENU:
            DisplayScreen_MainMenu(&touchs, setToDark);
            break;
        case DISP_CHANGE_SIGN_NAME:
            break;
        case DISP_SCR_ERROR:
            DisplayScreen_Error(&touchs, setToDark);
            break;

        default:
            break;
    }
} /* DisplayScreen */

/* -------------------------------------------------------------------------- */

/**
 * @brief   Initialize communication to the LCD display, graphical user 
 *          interface (GUI) and data 
 * @return  false if init failed
 */
bool DisplayInit() {

    /* Allumage backlight, demarrage des OC/PWM  */
    //DisplaySetBacklightRGB(COL_DEFAULT);
    //DRV_OC1_Start();    
    //DRV_OC2_Start();    
    //DRV_OC3_Start();   

    /* initalisation de la GUI, structure global et fonction pixel set callback */
    UG_Init(&disp.gui, DisplayPixelSetCallback, DISPLAY_WIDTH, DISPLAY_HEIGHT);

    /* init du LCD */
    bool isOk = LCD_Init();

    /* effacer le contenu de l'ecran en local (sera rafraichi periodiquement) */
    DisplayClear(true);
    disp.refreshTickEvent = false;
    disp.refreshDelayCount = 0;
    disp.currentScreenNr = 0;
    disp.graphTime = 0;

    /* init machine d'etat */
    disp.state = DISP_STATE_WAIT_REFRESH;
    disp.command = DISP_CMD_NOCMD;

    return isOk;

} /* DisplayInit */

/* -------------------------------------------------------------------------- */

/**
 * @brief   Task updates/refreshes the display, must be called periodically by the client
 * 
 */
void Display_Task() {
    if (LCD_Task_IsReady()) {
        switch (disp.state) {
            case DISP_STATE_WAIT_REFRESH:
            {
                if (disp.refreshTickEvent) {
                    disp.refreshTickEvent = false;

                    /* traitement en fonction de la commande en cours */
                    switch (disp.command) {
                        case DISP_CMD_NOCMD:
                        {
                            disp.page = 0;
                            break;
                        }
                        case DISP_CMD_SCROLLDOWN_CLEAR:
                        {
                            LCD_ScrollDown(DISPLAY_SCROLL_STEP);
                            DisplayClearPage(disp.scrollLine / DISPLAY_PAGE_HEIGHT, true);
                            disp.scrollLine += DISPLAY_SCROLL_STEP;
                            if (disp.scrollLine >= DISPLAY_HEIGHT) {
                                disp.command = DISP_CMD_NOCMD;
                            }
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                    disp.state = DISP_STATE_REFRESHING;
                }
                break;
            }

            case DISP_STATE_REFRESHING:
            { // Total Texec = 7 ms
                /* mise a jour de la page courante -> enverra une trame au LCD */
                LCD_DataWrite(&disp.screenCopy[disp.page][0], DISPLAY_WIDTH, disp.page, 0);
                disp.page++;
                if (disp.page >= DISPLAY_PAGES) {
                    disp.page = 0;
                    disp.state = DISP_STATE_WAIT_REFRESH;
                }
                break;
            }

            default:
            {
                disp.state = DISP_STATE_WAIT_REFRESH;
                break;
            }
        }
    }
} /* Display_Task */
///@}

void DrawEllipse(int centerX, int centerY, int a, int b) {
    int numSegments = 100; // Number of line segments to approximate the ellipse
    int i;
    for (i = 0; i < numSegments; i++) {
        float theta1 = 2.0f * M_PI * i / numSegments;
        float theta2 = 2.0f * M_PI * (i + 1) / numSegments;
        int x1 = centerX + a * cos(theta1);
        int y1 = centerY + b * sin(theta1);
        int x2 = centerX + a * cos(theta2);
        int y2 = centerY + b * sin(theta2);
        UG_DrawLine(x1, y1, x2, y2, C_BLACK);
    }
}

void DisplayScreen_Signals(uint16_t *stateTouch, bool setToDark) {

    char sginals[7][20];
    uint8_t i;
    if (stateTouch == NULL)
        return;
    for (i = 0; i < 7; i++) {
        strcpy(sginals[i], signalNames[i]);
        if (stateTouch[i] == 2) {
            strcat(sginals[i], " LN");
        } else if (stateTouch[i] != 0) {
            strcat(sginals[i], " ER");
        } else {
            strcat(sginals[i], " OK");
        }
    }
    if (setToDark) {
        UG_SetBackcolor(C_WHITE);
        UG_SetForecolor(C_BLACK);
    } else {
        UG_SetBackcolor(C_BLACK);
        UG_SetForecolor(C_WHITE);
    }

    UG_FontSetHSpace(0);

    /* Ecriture des chaines de caracteres */
    UG_FontSelect(&FONT_6X8);
    UG_PutString(0, 20, sginals[0]);
    UG_FontSelect(&FONT_6X8);
    UG_PutString(0, 35, sginals[1]);
    UG_FontSelect(&FONT_6X8);
    UG_PutString(0, 50, sginals[2]);
    UG_FontSelect(&FONT_6X8);
    UG_PutString(0, 67, sginals[3]);
    UG_FontSelect(&FONT_6X8);
    UG_PutString(0, 82, sginals[4]);
    UG_FontSelect(&FONT_6X8);
    UG_PutString(0, 99, sginals[5]);
    UG_FontSelect(&FONT_6X8);
    UG_PutString(0, 116, sginals[6]);
}

void SetSignalName(int index, const char* newName) {
    if (index >= 0 && index < 7) {
        strncpy(signalNames[index], newName, sizeof (signalNames[index]) - 1);
        signalNames[index][sizeof (signalNames[index]) - 1] = '\0'; // Sécurité
    }
}

void EditSignalName_IHM(int index) {
    char tempName[20];
    strncpy(tempName, signalNames[index], sizeof (tempName));
    tempName[sizeof (tempName) - 1] = '\0';
    int pos = 0; // Position du curseur dans le nom
    int editing = 1;
    uint16_t lastTouch = 0;
    while (editing) {
        // Afficher le nom en cours d'édition avec un curseur (ex: '_')
        char display[24];
        snprintf(display, sizeof (display), "%s_", tempName);
        UG_FillFrame(0, 0, 127, 15, C_WHITE); // Efface la zone d'affichage
        UG_SetForecolor(C_BLACK);
        UG_PutString(0, 0, display);
        // Affiche le curseur visuel
        UG_DrawLine(0 + pos * 8, 12, 7 + pos * 8, 12, C_BLACK); // Soulignement sous le caractère courant

        // Attendre une entrée utilisateur (ici, on suppose un polling du touchStates global)
        extern volatile uint16_t g_lastTouchStates; // À déclarer dans ton projet, ou passer en paramètre
        uint16_t touch = g_lastTouchStates;
        // Gestion des touches :
        // - KEY_UP_C_MASK : lettre suivante (A->B)
        // - KEY_DOWN_C_MASK : lettre précédente (B->A)
        // - KEY_UP_R_MASK : curseur à droite
        // - KEY_DOWN_R_MASK : curseur à gauche
        // - KEY_MID_L_MASK : valider
        // - KEY_MID_R_MASK : annuler
        if ((touch & KEY_UP_C_MASK) && !(lastTouch & KEY_UP_C_MASK)) {
            if (tempName[pos] == '\0' || tempName[pos] == ' ') tempName[pos] = 'A';
            else if (tempName[pos] >= 'A' && tempName[pos] < 'Z') tempName[pos]++;
            else tempName[pos] = 'A';
        }
        if ((touch & KEY_DOWN_C_MASK) && !(lastTouch & KEY_DOWN_C_MASK)) {
            if (tempName[pos] == '\0' || tempName[pos] == ' ') tempName[pos] = 'Z';
            else if (tempName[pos] > 'A' && tempName[pos] <= 'Z') tempName[pos]--;
            else tempName[pos] = 'Z';
        }
        if ((touch & KEY_UP_R_MASK) && !(lastTouch & KEY_UP_R_MASK)) {
            if (pos < (int) strlen(tempName) - 1 && pos < (int) sizeof (tempName) - 2) pos++;
        }
        if ((touch & KEY_DOWN_R_MASK) && !(lastTouch & KEY_DOWN_R_MASK)) {
            if (pos > 0) pos--;
        }
        if ((touch & KEY_MID_L_MASK) && !(lastTouch & KEY_MID_L_MASK)) {
            // Valider
            editing = 0;
        }
        if ((touch & KEY_MID_R_MASK) && !(lastTouch & KEY_MID_R_MASK)) {
            // Annuler
            return;
        }
        lastTouch = touch;


    }
    // Appliquer le nouveau nom
    SetSignalName(index, tempName);
}

/* *****************************************************************************
 End of File Display
 */
