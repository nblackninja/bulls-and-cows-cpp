// Bulls and Cows.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <ctime>
#include <random>
#include <windows.h>
#include <cmath>
#include <fstream>
using namespace std;

#define DATE_CREATE 25

void clear(); // очистка консоли
int randNumFunc(const int min, const int max); // случайное число в диапозоне
string NowDate(); // выводит полную дату

// список цветов
enum ConsoleColor;
// устанавливает цвет текста и подложки в консоли
void SetColor(int text, int background);
// устанавливает цвет текста и подложки в консоли 
void SetColor(int text, ConsoleColor/*int*/ background);

// меню
int Menu_Item(); 
void Menu_LoadGame();
void Menu_Settings(int& GodMode, int& colorText, int& colorBG);
void Menu_Author();
bool Menu_ExitGame();

// Коровы - вывод на экран цифр, которые есть в числе
void SearchCows(int count, int TurnDigit, int countUser);

// Быки - вывод на экран цифр, которые есть в числе и стоят в нужной позиции
void SearchBulls(int count, int TurnDigit, int countUser);

// сложность игры
void difGame(int VarDifGame, int& pointsUser, int& TurnCount);

// компьютер, который загадывает число
class EnemyPC
{
public:
	EnemyPC(int TurnDigit)
	{
		this->TurnDigit = TurnDigit;
	};

	// создаем максимальное число с заданной длинной (пример:999)
	int MaxNumFromDigit(int TurnDigit)
	{
		int MaxNum = 9;
		for (int i = 1; i < TurnDigit; i++)
		{
			MaxNum = MaxNum * 10 + 9;
		}
		return MaxNum;
	}

	// возвращает случайное число с заданным количеством цифр
	int RandomNumberPCFunc(int TurnDigit)
	{
		int min = pow(10, (TurnDigit - 1));
		int max = MaxNumFromDigit(TurnDigit);
		return randNumFunc(min, max);
	}

private:
	int TurnDigit;
};

// обычный пользователь
class UserUser
{
public:
	UserUser(int TurnCount, int TurnDigit, int GodMode)
	{
		userName = "NoName";
		this->TurnCount = TurnCount;
		this->TurnDigit = TurnDigit;
		this->GodMode = GodMode;
	}

	void SetName(string userName)
	{
		this->userName = userName;
	}
	void SetTurnCount(int TurnCount)
	{
		this->TurnCount = TurnCount;
	}
	void SetTurnDigit(int TurnDigit)
	{
		this->TurnDigit = TurnDigit;
	}
	void SetGodMode(int GodMode)
	{
		this->GodMode = GodMode;
	}
	void SetAllPointsUser(int allPointsUser)
	{
		this->allPointsUser = allPointsUser;
	}
	void SetCountGameRound(int countGameRound)
	{
		this->countGameRound = countGameRound;
	}
	void SetTextColor(int textColor)
	{
		this->textColor = textColor;
	}
	void SetBgColor(int bgColor)
	{
		this->bgColor = bgColor;
	}
	void SetVarDifGame(int varDifGame)
	{
		this->varDifGame = varDifGame;
	}


	string GetName()
	{
		return userName;
	}
	int GetTurnCount()
	{
		return TurnCount;
	}
	int GetTurnDigit()
	{
		return TurnDigit;
	}
	int GetGodMode()
	{
		return GodMode;
	}
	int GetAllPointsUser()
	{
		return allPointsUser;
	}
	int GetCountGameRound()
	{
		return countGameRound;
	}
	int GetTextColor()
	{
		return textColor;
	}
	int GetBgColor()
	{
		return bgColor;
	}
	int GetVarDifGame()
	{
		return varDifGame;
	}


private:
	string userName;

	int TurnCount,
		TurnDigit,
		allPointsUser = 0,
		countGameRound,
		textColor,
		bgColor,
		GodMode,
		varDifGame;
};

// Сохраняет данные в файл autosave
void SaveGame(UserUser& myClass, string Date);

void LoadGame(UserUser& myClass);

int main()
{
	setlocale(LC_ALL, "Russian");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	system("title Bulls and Cows");
	system("mode con cols=82 lines=16");
	
	// размер шрифта
	{
		CONSOLE_FONT_INFOEX cfi;
		cfi.cbSize = sizeof cfi;
		cfi.nFont = 0;
		cfi.dwFontSize.X = 0;
		cfi.dwFontSize.Y = 26;
		cfi.FontFamily = FF_DONTCARE;
		cfi.FontWeight = FW_NORMAL;
		wcscpy_s(cfi.FaceName, L"Consolas");
		SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
	}

	// settings
	int TurnCount = 10; // количество ходов
	const int TurnDigit = 4;
	int GodMode = 0;

	// settings - new game
	string userName;
	int pointsUser = 0, // количество очков
		turnUser = 0,
		allPointsUser = 0,
		varDifGame,
		countGameRound = 0;
	int textColor = 7,
		bgColor = 0;

	string Date = NowDate();
	UserUser User(TurnCount, TurnDigit, GodMode);

	string saveName;
	int counterPointSave;
	int answerLoadGame;

	while (true)
	{
		newGame:
		int answer = Menu_Item();

		if (answer == 1)
		{
			{
				ifstream file;
				file.open("autosave.txt");


				if (file)
				{
					clear();
					cout << "У вас есть сохранение: ";
					file >> saveName;
					cout << saveName;
					file >> counterPointSave;
					cout << " | очков = " << counterPointSave << endl;
					file.close();

					cout << "Загрузить сохранение? (0 - нет, 1 - да)\n>>> ";
					cin >> answerLoadGame;
					if (answerLoadGame == 1)
					{
						LoadGame(User);
						countGameRound = User.GetCountGameRound();
						textColor = User.GetTextColor();
						bgColor = User.GetBgColor();
						SetColor(textColor, bgColor);
						goto StartGame;
					}
				}
			}

			clear();
			cout << "Режим игры" << endl << endl;
			cout << "1. Легкий" << endl;
			cout << "Ходов = 10\nОчков со старта = 4096" << endl << endl;
			cout << "2. Средний" << endl;
			cout << "Ходов = 5\nОчков со старта = 8192" << endl << endl;
			cout << "3. Тяжелый" << endl;
			cout << "Ходов = 3\nОчков со старта = 16384\n\n>>> ";
			cin >> varDifGame;

			User.SetVarDifGame(varDifGame);

			clear();

			User.SetTextColor(textColor);
			User.SetBgColor(bgColor);

			cout << "Ваше имя: ";
			cin >> userName;
			User.SetName(userName);

			User.SetTurnCount(TurnCount);
			User.SetGodMode(GodMode);

			StartGame:
			bool againGame = true;
			while (againGame == true)
			{
				EnemyPC Enemy(TurnDigit);
				int GameRandNum = Enemy.RandomNumberPCFunc(TurnDigit);

				int answerNumber;
				bool answerTrue = false;
				varDifGame = User.GetVarDifGame();
				difGame(varDifGame, pointsUser, TurnCount);

				clear();
				while (answerTrue == false)
				{
					if (User.GetGodMode() == 1)
						cout << "Правильное [" << GameRandNum << "]" << endl;

					cout << "\nВаше число\n>>> ";
					cin >> answerNumber;
					cout << endl;

					if (answerNumber == GameRandNum)
					{
						clear();
						cout << "Поздравляю, " << User.GetName() << "! Вы победили! =)" << endl;

						if (User.GetAllPointsUser() >= 0)
						{
							allPointsUser = User.GetAllPointsUser();
						}
						allPointsUser = allPointsUser + pointsUser;
						User.SetAllPointsUser(allPointsUser);
						cout << "Всего очков: " << User.GetAllPointsUser() << endl;

						cout << "Ход = " << turnUser << " из " << TurnCount << endl;

						countGameRound++;
						User.SetCountGameRound(countGameRound);
						cout << "Раунд = " << User.GetCountGameRound() << endl;
					
						SaveGame(User, Date);

						cout << "\n\nЕще? (0 - нет, 1 - да)";
						cin >> againGame;
						answerTrue = true;
						turnUser = 0;
					}
					else if (answerNumber != GameRandNum)
					{
						clear();
						cout << "Коровы: ";
						SearchCows(GameRandNum, TurnDigit, answerNumber);
						cout << endl;

						cout << "Быки: ";
						SearchBulls(GameRandNum, TurnDigit, answerNumber);
						cout << endl;

						turnUser++;
						cout << "Ход = " << turnUser << " из " << TurnCount << endl;
						pointsUser /= 2;
						cout << "Очки = " << (pointsUser) << endl;

						if (turnUser == TurnCount)
						{
							clear();
							cout << User.GetName() << "! Вы проиграли :(" << endl << endl;
							cout << "Ходов = " << turnUser << " из " << TurnCount << endl;
							pointsUser = 0;
							answerTrue = true;

							if (User.GetAllPointsUser() > 0)
							{
								allPointsUser = User.GetAllPointsUser();
							}
							allPointsUser += pointsUser;
							User.SetAllPointsUser(allPointsUser);
							cout << "Всего очков: " << User.GetAllPointsUser() << endl;

							countGameRound++;
							User.SetCountGameRound(countGameRound);
							cout << "Раунд = " << User.GetCountGameRound() << endl;

							SaveGame(User, Date);

							int againGame;
							cout << "Еще? (0 - нет, 1 - да)";
							cin >> againGame;
							if (againGame == 0)
							{
								goto newGame;
							}
							turnUser = 0;
						}
					}

				}
			}
		}
		else if (answer == 2)
		{
			Menu_LoadGame();

			ifstream file;
			file.open("autosave.txt");

			if (file)
			{
				clear();
				cout << "У вас есть сохранение: ";
				file >> saveName;
				cout << saveName;
				file >> counterPointSave;
				cout << " | очков = " << counterPointSave << endl;
				file.close();

				cout << "Загрузить сохранение? (0 - нет, 1 - да)\n>>> ";
				cin >> answerLoadGame;
				if (answerLoadGame == 1)
				{
					LoadGame(User);
					countGameRound = User.GetCountGameRound();
					textColor = User.GetTextColor();
					bgColor = User.GetBgColor();
					SetColor(textColor, bgColor);
					goto StartGame;
				}
			}
			else
			{
				cout << "Сохранений не найдено..." << endl;
				system("pause");
			}
		}
		else if (answer == 3)
		{
			Menu_Settings(GodMode, textColor, bgColor);
		}
		else if (answer == 4)
		{
			Menu_Author();
		}
		else if (answer == 5)
		{
			clear();
			cout << "Цель игры: угадать 4-значное число за определенное количество попыток." << endl;
			cout << "Коровы - цифра угадана, но её позиция — нет" << endl;
			cout << "Быки - совпадает и цифра, и её позиция." << endl;
			cout << "Например, если загадано число «1234», а вы называете «6531»" << endl;
			cout << "то результатом будет 1 корова (цифра «1») и 1 бык (цифра «3»)." << endl << endl;
			system("pause");
		}
		else if (answer == 6)
		{
			break;
		}
	}

	clear();
	cout << "Пока..." << endl << endl;
	Menu_ExitGame(); // закрыть консоль

    return 0;
}

// Функции

int Menu_Item()
{
	clear();

	cout << " _____ _____ _ _ _ _____    _____ _____ ____     _____ _____ __    __    _____ " << endl;
	cout << "|     |     | | | |   __|  |  _  |   | |    \\   | __  |  |  |  |  |  |  |   __|" << endl;
	cout << "|   --|  |  | | | |__   |  |     | | | |  |  |  | __ -|  |  |  |__|  |__|__   |" << endl;
	cout << "|_____|_____|_____|_____|  |__|__|_|___|____/   |_____|_____|_____|_____|_____|" << endl;
	cout << "                                                                               " << endl;

	// Выбранный вариант меню
	int variant;
	cout << setw(30) << "Меню\n" << endl;
	cout << setw(34) << "1. Новая игра\n"
		<< setw(38) << "2. Загрузить игру\n"
		<< setw(33) << "3. Настройки\n"
		<< setw(30) << "4. Авторы\n"
		<< setw(31) << "5. Правила\n"
		<< setw(29) << "6. Выйти\n" << endl;
	cout << ">>> ";
	cin >> variant;
	return variant;
}

// 2
void Menu_LoadGame()
{
	clear();
	cout << "Выберите сохранение\n" << endl;
}

// 3
void Menu_Settings(int& GodMode, int& colorText, int& colorBG)
{
	clear();
	cout << "Настройки игры\n" << endl;
	cout << "Список цветов:" << endl;
	cout << "0 - черный\n1 - голубой\n2 - зеленый\n3 - синий\n4 - красный\n7 - серый\n15 - белый\n\n";
	cout << "1. Цвет текста: ";
	cin >> colorText;
	cout << "2. Фон под текстом: ";
	cin >> colorBG;
	cout << "3. Режим бога (0 - нет, 1 - да): ";
	cin >> GodMode;
	cout << endl;
	SetColor(colorText, colorBG);
}

// 4
void Menu_Author()
{
	clear();
	cout << "Автор игры - Никита Юдин\nДата создания: " << DATE_CREATE << " ноября 2018 года\n" << endl;
	system("pause");
}

// 5
bool Menu_ExitGame()
{
	return EXIT_SUCCESS;
}

// игровые
void SearchCows(int count, int TurnDigit, int countUser)
{
	int delitel = 1,
		countNum,
		procNum;

	int delitelUser = 1,
		countNumUser,
		procNumUser;

	int counterMas = 0;

	int *arrNum = new int[TurnDigit]; // Выделение памяти для массива
	int k = 0;
	for (int i = 0; i < TurnDigit; i++)
	{
		arrNum[i] = -1;
	}


	for (int i = 1; i < TurnDigit + 1; i++)
	{
		procNum = pow(10, i);
		if (i > 1)
			delitel = pow(10, i - 1);
		countNum = (count % procNum / delitel);

		for (int j = 1; j < TurnDigit + 1; j++)
		{
			procNumUser = pow(10, j);
			if (j > 1)
				delitelUser = pow(10, j - 1);
			countNumUser = (countUser % procNumUser / delitelUser);

			int haveCount = 0;
			if (countNumUser == countNum)
			{
				// проверка, содержится ли угаданная цифра в массиве
				for (int k = 0; k < TurnDigit; k++)
				{

					if (arrNum[k] == countNum)
					{
						break;
					}
					// проверяем, есть ли число в массиве
					if (arrNum[k] != countNum)
					{
						for (int kk = 0; kk < TurnDigit; kk++)
						{
							if (arrNum[kk] == countNum)
							{
								haveCount = 1;
							}
						}
						if (haveCount == 0)
						{
							arrNum[counterMas] = countNum;
							counterMas++;
							break;
						}
					}
					haveCount = 0;
				}
				// cout << countNum << " ";
			}
		}
		delitelUser = 1;
	}

	// вывод массива с коровыми
	for (int i = 0; i < TurnDigit; i++)
	{
		if (arrNum[i] > -1)
		{
			cout << arrNum[i] << " ";
		}
	}

	delete[] arrNum; // очистка памяти
}
void SearchBulls(int count, int TurnDigit, int countUser)
{
	// массив для числа пользователя
	int *arrNumUser = new int[TurnDigit];
	for (int i = 0; i < TurnDigit; i++)// заполнение числа с конца
	{
		arrNumUser[i] = countUser % 10;
		countUser /= 10;
	}

	// массив для числа ПК
	int *arrNumPC = new int[TurnDigit];
	for (int i = 0; i < TurnDigit; i++)// заполнение числа с конца
	{
		arrNumPC[i] = count % 10;
		count /= 10;
	}

	// проверка на совпадение по индексу
	for (int i = TurnDigit - 1; i >= 0; i--)
	{
		if (arrNumUser[i] == arrNumPC[i])
		{
			cout << arrNumUser[i];
		}
		else
		{
			cout << '*';
		}
	}

	// очистка памяти
	delete[] arrNumUser;
	delete[] arrNumPC;
}
void difGame(int VarDifGame, int& pointsUser, int& TurnCount)
{
	if (VarDifGame == 1)// легкий
	{
		pointsUser = 4096;
		TurnCount = 10;
	}
	else if (VarDifGame == 2) // средний
	{
		pointsUser = 8192;
		TurnCount = 5;
	}
	else if (VarDifGame == 3) // тяжелый
	{
		pointsUser = 16384;
		TurnCount = 3;
	}
	else
	{
		cout << "Ошибка! Неправильно указана сложность!" << endl;
	}
}


void SaveGame(UserUser& myClass, string Date)
{
	// Имя, Всего очков, Количество ходов, Количество цифр, Режим бога, Количество раундов
	// Цвет текста, Цвет фона, Уровень сложности
	ofstream inFile("autosave.txt");
	if (!inFile) cout << "Блин! Не удалось создать файл сохранения!";
	// inFile << Date << endl;
	inFile << myClass.GetName() << endl;
	inFile << myClass.GetAllPointsUser() << endl;
	inFile << myClass.GetTurnCount() << endl;
	inFile << myClass.GetTurnDigit() << endl;
	inFile << myClass.GetGodMode() << endl;
	inFile << myClass.GetCountGameRound() << endl;
	inFile << myClass.GetTextColor() << endl;
	inFile << myClass.GetBgColor() << endl;
	inFile << myClass.GetVarDifGame() << endl;
	inFile.close();
}
void LoadGame(UserUser& myClass)
{
	string name;

	ifstream outFile("autosave.txt");
	int i = 0;
	while (i == 0)
	{
		outFile >> name;
		cout << name << endl;
		i++;
	}

	myClass.SetName(name);

	int num;

	while (i <= 9)
	{
		outFile >> num;

		if (i == 1)
		{
			i++;
		}

		if(i == 2)
		{
			cout << num << endl;
			myClass.SetAllPointsUser(num);
			i++;
		}
		else if (i == 3)
		{
			cout << num << endl;
			myClass.SetTurnCount(num);
			i++;
		}
		else if (i == 4)
		{
			cout << num << endl;
			myClass.SetTurnDigit(num);
			i++;
		}
		else if (i == 5)
		{
			cout << num << endl;
			myClass.SetGodMode(num);
			i++;
		}
		else if (i == 6)
		{
			cout << num << endl;
			myClass.SetCountGameRound(num);
			i++;
		}
		else if (i == 7)
		{
			cout << num << endl;
			myClass.SetTextColor(num);
			i++;
		}
		else if (i == 8)
		{
			cout << num << endl;
			myClass.SetBgColor(num);
			i++;
		}
		else if (i == 9)
		{
			cout << num << endl;
			myClass.SetVarDifGame(num);
			i++;
		}
	}

	outFile.close();
}


// вспомогательные
int randNumFunc(const int min, const int max)
{
	random_device rd;
	mt19937 randomAllTime(rd());
	uniform_int_distribution<int> randomNumPointsMinMax(min, max);
	int randomNumPoints = randomNumPointsMinMax(randomAllTime);

	return randomNumPoints;
}
void clear() {
	COORD topLeft = { 0, 0 };
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO screen;
	DWORD written;

	GetConsoleScreenBufferInfo(console, &screen);
	FillConsoleOutputCharacterA(
		console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);
	FillConsoleOutputAttribute(
		console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
		screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);
	SetConsoleCursorPosition(console, topLeft);
};
enum ConsoleColor
{
	Black = 0,
	Blue = 1,
	Green = 2,
	Cyan = 3,
	Red = 4,
	Magenta = 5,
	Brown = 6,
	LightGray = 7,
	DarkGray = 8,
	LightBlue = 9,
	LightGreen = 10,
	LightCyan = 11,
	LightRed = 12,
	LightMagenta = 13,
	Yellow = 14,
	White = 15
};
void SetColor(int text, int background)
{
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
};
void SetColor(int text, ConsoleColor/*int*/ background)
{
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
}
string NowDate()
{
	char buffer[80];
	time_t seconds = time(NULL);
	tm* timeinfo = localtime(&seconds);
	char* format = "%d %B | %Y | %I:%M:%S";
	strftime(buffer, 80, format, timeinfo);
	return buffer;
}
