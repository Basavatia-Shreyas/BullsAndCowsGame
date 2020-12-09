// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    Isograms = GetValidWords(Words);
    SetupGame();//set up the game
}

void UBullCowCartridge::OnInput(const FString& PlayerInput) // When the player hits enter
{
    if (bGameOver)
    {
        ClearScreen();
        SetupGame();
    }
    else
    {
        ProcessGuess(PlayerInput);
    }
}

void UBullCowCartridge::SetupGame()
{
    
    HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num() - 1)];
    Lives = HiddenWord.Len() * 2;
    bGameOver = false;

    PrintLine(TEXT("Welcome to the Bull Cow Game"));// it is wrapped in TEXT macro which is needed for unreal to play nicley with all platfroms it supports.
    PrintLine(TEXT("Guess the %i letter word"), HiddenWord.Len());
    PrintLine(TEXT("You have %i lives"), Lives);
    PrintLine(TEXT("Type in your guess and press enter"));//wrap all text and strings no matter where in TEXT macro
    //PrintLine(TEXT("The Hidden Word is: %s"), *HiddenWord);//debugLine
}

void UBullCowCartridge::Endgame()
{
    bGameOver= true;
    PrintLine(TEXT("\nPress enter to play again"));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)//this makes Guess a reference to what the value of guess is instead of making a copy(saves resources)
{
    if(Guess == HiddenWord)//we can do == bc they are both fstrings
        {
            PrintLine(TEXT("You Win!"));
            Endgame();
            return;
        }

        if (HiddenWord.Len() != Guess.Len())//gets length of the string of hiddenword and player input
        {
            PrintLine(TEXT("Sorry try guessing again \nYou have %i lives remaining"), Lives);
            PrintLine(TEXT("The hidden word is %i letters long"), HiddenWord.Len());
            return;
        }

        //check if not isogram
        if (!IsIsogram(Guess))
        {
            PrintLine(TEXT("There are no repeating letters, guess agian"));
            return;
        }

        PrintLine(TEXT("You have lost a life"));
        --Lives;
        if (Lives <= 0)
        {
            ClearScreen();
            PrintLine(TEXT("You have lost"));
            PrintLine(TEXT("The hidden word was %s"), *HiddenWord);
            Endgame();
            return;
        }

        FBullCowCount Score = GetBullCows(Guess);

        PrintLine(TEXT("You have %i Bulls and %i Cows"), Score.Bulls, Score.Cows);

        PrintLine(TEXT("Guess again, you have %i lives left"), Lives);
}

bool UBullCowCartridge::IsIsogram(const FString& Word) const
{
    
    for (int32 Index = 0; Index < Word.Len() - 1; Index++)
    {
        for (int32 Comparison = Index + 1; Comparison < Word.Len(); Comparison++)
        {
            if (Word[Index] == Word[Comparison])
            {
                //PrintLine(TEXT("That is not an Isogram, try again"));
                return false;
            }
        }
    }
    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const
{
    TArray<FString> ValidWords;

    for (FString Word : WordList)
    {   
        if (Word.Len() >= 4 && Word.Len() <= 8 && IsIsogram(Word))
        {
            ValidWords.Emplace(Word);    
        }
    }
    return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{
    FBullCowCount Count;

    for (int32 i = 0; i < Guess.Len(); i++)
    {
        if(Guess[i] == HiddenWord[i])
        {
            Count.Bulls++;
            continue;//skips the rest of the code and returns to the for loop
        }

        for (int32 j = 0; j < HiddenWord.Len(); j++)
        {
            if (Guess[i] == HiddenWord[j])
            {
                Count.Cows++;
                break;//skips the rest of the for loop and goes back
            }
        }
    }
    return Count;
}
