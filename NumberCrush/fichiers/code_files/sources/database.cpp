#include "../headers/database.h"
#include "../headers/tools.h"
#include "../headers/struct.h"
#include "../headers/boolean_tests.h"

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <stdio.h>
#include <vector>
#include <unistd.h>

using namespace std;

namespace Level {
    void SaveScore (const string & KLevelScoreFileName, const string & KPlayerPseudo, const unsigned & KPoints)
    {
        ofstream score(KLevelScoreFileName);

        score << "BestScore : " << KPoints << endl
              << " Player : " << KPlayerPseudo; // write the best score and the pseudo of the player into the file

        score.close();
    }//SaveScore

    void ReadBestScore (GameSettings & Params)
    {
        ifstream score(Params.LevelScoreFileName);

        Params.MapLevelInfosString.clear();
        Params.MapLevelInfosUnsigned.clear();

        if (!score.is_open ()) // if the file for this level does not exist
        {
            cerr << "Aucun meilleur score enregistre pour ce niveau !" << endl;
        }
        else
        {
            string Key;
            while (score >> Key)
            {
                char tmp;
                score >> tmp;

                if (find (KAuthorizedKey.VLevelInfosUnsigned.begin(), KAuthorizedKey.VLevelInfosUnsigned.end(), Key) != KAuthorizedKey.VLevelInfosUnsigned.end())
                    score >> Params.MapLevelInfosUnsigned[Key];
                else if (find (KAuthorizedKey.VLevelInfosString.begin(), KAuthorizedKey.VLevelInfosString.end(), Key) != KAuthorizedKey.VLevelInfosString.end())
                    score >> Params.MapLevelInfosString[Key];
            }

            if (Params.MapLevelInfosString.size() + Params.MapLevelInfosUnsigned.size() != Params.KNbBestScoreParams)
            {
                cerr << "Erreur detectee dans le fichier" << endl;
                Params.MapLevelInfosString.clear();
                Params.MapLevelInfosUnsigned.clear();
            }
        }
        score.close();
    }//ReadBestScore

    void LoadLevelParams (GameSettings & Params)
    {
        ifstream RunningLevel (Params.LevelConfigFileName); // open the called level file

        if (!RunningLevel.is_open())
        {
            cerr << "Aucun fichier de configuration pour ce niveau !" << endl;
            exit (2);
        }

        string Key;
        while (RunningLevel >> Key)
        {
            char tmp;
            vector <CPosition> WallPositions;
            CPosition Pos;

            RunningLevel >> tmp; // go over ':'

            if (find (KAuthorizedKey.VParamUnsigned.begin(), KAuthorizedKey.VParamUnsigned.end(), Key) != KAuthorizedKey.VParamUnsigned.end())
                RunningLevel >> Params.MapParamUnsigned[Key];
            else if (find (KAuthorizedKey.VParamBool.begin(), KAuthorizedKey.VParamBool.end(), Key) != KAuthorizedKey.VParamBool.end())
                RunningLevel >> Params.MapParamBool[Key];
            else if (find (KAuthorizedKey.VParamCVPosition.begin(), KAuthorizedKey.VParamCVPosition.end(), Key) != KAuthorizedKey.VParamCVPosition.end())
            {
                RunningLevel >> tmp; // go over '['
                while (true)
                {
                    string Str;
                    RunningLevel >> Str;

                    if (IsValidUsignedValue(Str, 0, 26))
                    {
                        Pos.first = stoul(Str);
                        RunningLevel >> tmp; // go over ';'
                        RunningLevel >> Pos.second;
                        WallPositions.push_back(Pos);
                    }
                    else if (Str == "]") break; // end of wall positions
                    else
                    {
                        cerr << "Le fichier charge contient des informations invalides !" << endl;
                        exit(2);
                    }
                }
                Params.MapParamCVPosition[Key] = WallPositions;
            }
        }

        if (Params.MapParamUnsigned.size() != Params.KNbUnsignedParams
            || Params.MapParamBool.size() != Params.KNbBoolParams
            || Params.MapParamCVPosition.size() != Params.KNbVCPosParams)
        {
            cerr << "Le fichier charge ne contient pas les informations necessaires !" << endl;
            exit(2);
        }
    }//LoadLevelParams
}//namespace Level

namespace Player {
    void SaveLevel (PlayerInfos & Player, const unsigned & KCurrentLevel)
    {
        if (KCurrentLevel >= Player.MapPlayerUnsigned.at("HigherLevel")) // if level win by the player is the higher he can plays on
            Player.MapPlayerUnsigned.at("HigherLevel") = KCurrentLevel + 1; // add a level

        ofstream profil(Player.PlayerFileName);

        profil << "HigherLevel : " << Player.MapPlayerUnsigned.at("HigherLevel") << endl; // save player levels into the file

        profil.close();
    }//SaveLevel

    void CreateProfile (const string & KPlayerFileName)
    {
        ofstream profil(KPlayerFileName, ios_base::trunc);

        profil << "HigherLevel : " << 1 << endl; // set highest level reach to 1

        profil.close();
    }//CreateProfile

    void LoadProfile (PlayerInfos & Player, const GameSettings & KParams)
    {
        ifstream profil(Player.PlayerFileName);

        if (!profil.is_open())
        {
            cerr << "Aucun profil correspondant." << endl << endl;
            CreateProfile(Player.PlayerFileName); // create a profile for the player if it does not exist
            cout << "Bienvenue, " << Player.PlayerPseudo << " !" << endl;
            cout << "Votre profil a ete cree." << endl << endl;
            profil.open(Player.PlayerFileName);
        }
        string Key;
        while (profil >> Key)
        {
            char tmp;
            profil >> tmp;

            if (find (KAuthorizedKey.VPlayerUnsigned.begin(), KAuthorizedKey.VPlayerUnsigned.end(), Key) != KAuthorizedKey.VPlayerUnsigned.end())
                profil >> Player.MapPlayerUnsigned[Key];

            if (Player.MapPlayerUnsigned.size() != Player.KNbUnsignedParams)
            {
                cerr << "Erreur detectee dans le fichier joueur !" << endl;
                CreateProfile(Player.PlayerFileName); // create profile again if file contains errors
                cout << "Votre fichier a ete reinitialise." << endl;
                LoadProfile(Player, KParams);
                return;
            }
        }

            if (Player.MapPlayerUnsigned.at("HigherLevel") == 0)
                Player.MapPlayerUnsigned.at("HigherLevel") = KParams.KNumberOfLevels; // 0 means all levels unlocked

            cout << "Le profil '" << Player.PlayerPseudo << "' a bien ete charge !" << endl;
    }//LoadProfile
}//namespace Player

void ReadFile (const string & KFil)
{
    ifstream File;
    File.open(KFil);
    string line;

    if (!File.is_open())
        cerr << "Ce fichier n'existe pas !" << endl;
    else
        while(!File.eof())
        {
            getline(File, line);
            cout << line << endl;
        }

    File.close();
}//ReadFile

bool AllDirectoriesExists (const vector<const char *> & KDirectoriesName)
{
    int DirectoryValid;


        for (const char * dir : KDirectoriesName)
        {
            DirectoryValid = access(dir, F_OK); // does the directory exist ?
            if (DirectoryValid != 0) // 0 means yes
            {
                cerr << "Dossier(s) manquant(s) !" << endl;
                return false;
            }
        }
        return true;
}//AllDirectoriesExists
