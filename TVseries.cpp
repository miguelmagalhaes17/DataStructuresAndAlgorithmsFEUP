#include "TVseries.hpp"
#include <iostream>
#include <algorithm>
#include <functional>
#include <stack>
#include <map>
#include <unordered_map>
#include <unordered_set>


TVSeriesAPP::TVSeriesAPP() {

}
   
TVSeriesAPP::~TVSeriesAPP() {
    
}


void TVSeriesAPP::addTitleBasics(const TitleBasics& title) { titleBasics.push_back(title); }

void TVSeriesAPP::addTitleEpisodes(const TitleEpisode& episode) { titleEpisodes.push_back(episode); }

void TVSeriesAPP::addTitlePrincipal(const TitlePrincipals& principal) { titlePrincipals.push_back(principal); }



//Pergunta 1:

vector<string> TVSeriesAPP::getUniquePrincipals(const string& seriesTconst ) const {
    vector<string> uniquePrincipals; // para guardar os nomes dos atores principais
    vector<string> uniqueNconsts; // para guardar os nconsts dos atores principais

    for (const auto& episode : titleEpisodes) {
        if (episode.parentTconst == seriesTconst) {
            for (const auto& principal : titlePrincipals) {
                if (principal.tconst == episode.tconst) {
                    if (find(uniqueNconsts.begin(), uniqueNconsts.end(), principal.nconst) == uniqueNconsts.end()) {
                        uniqueNconsts.push_back(principal.nconst);
                        uniquePrincipals.push_back(principal.primaryName);
                    }
                }
            }
        }
    }

    sort(uniquePrincipals.begin(), uniquePrincipals.end());

    return uniquePrincipals;

}

//PERGUNTA 2:



string TVSeriesAPP::getMostSeriesGenre() const {

    map<string, int> genreCount; // key = genero, value = numero de vezes que aparece
    
    // adicionar o numero de vezes que cada genero aparece
    for (const auto& title : titleBasics) {
        for (const auto& genre : title.genres) {
            genreCount[genre]++; // genre é a key, e o value é o numero de vezes que aparece
        }
    }

    string mostPopularGenre;
    int maxCount = 0;

    // encontrar o genero mais popular
    for (const auto& entry : genreCount) {
        const string& genre = entry.first;
        int count = entry.second;

        if (count > maxCount || (count == maxCount && genre.length() < mostPopularGenre.length())) {
            mostPopularGenre = genre;
            maxCount = count;
        }
    }

    return mostPopularGenre;
}


//PERGUNTA 3: 

string TVSeriesAPP::getPrincipalFromCharacter(const string& character) const {

    if (character.empty()) {
        return "";
    }

    map<string, int> characterCount; // key = nconst, value = numero de vezes que aparece
    map<string, string> nameMap;     // key = nconst, value = primaryName

    // quanta vezes a personagem foi interpretada por cada pessoa
    for (const auto& principal : titlePrincipals) {
        for (const auto& charName : principal.characters) {
            if (charName == character) {
                characterCount[principal.nconst]++;
                nameMap[principal.nconst] = principal.primaryName;
            }
        }
    }

    // quem interpretou mais vezes a personagem
    string topPrincipal;
    int maxCount = 0;

    for (const auto& entry : characterCount) {
        const string& nconst = entry.first;
        int count = entry.second;

        // enunciado: If the count results in a tie, the person’s name with higher alphabetical order 3 is selected.

        if (count > maxCount || (count == maxCount && nameMap[nconst] > nameMap[topPrincipal])) {
            topPrincipal = nconst;
            maxCount = count;
        }
    }

    if (topPrincipal.empty()) {
        return "";
    } else {
        return nameMap[topPrincipal];
    }

}



//PERGUNTA 4

vector<string> TVSeriesAPP::principalsWithMultipleCategories(const string& seriesTconst ) const {

    bool seriesExists = false;
    for (const auto& title : titleBasics) {
        if (title.tconst == seriesTconst) {
            seriesExists = true;
            break;
        }
    }
    
    unordered_map<string, unordered_set<string>> principalCategories; // key = nconst, value = set of categories, usar set para garantir que não há repetições

    // encontrar episodios que pertencem à serie
    for (const auto& episode : titleEpisodes) {
        if (episode.parentTconst == seriesTconst) {
            // encontrar os atores principais do episodio
            for (const auto& principal : titlePrincipals) {
                if (principal.tconst == episode.tconst) {
                    // adicionar a categoria do ator principal ao set
                    principalCategories[principal.nconst].insert(principal.category);
                }
            }
        }
    }

    // vetor para guardar os nomes dos atores principais com mais de uma categoria
    vector<string> result;

    // ver no map quais os atores principais que têm mais de uma categoria
    for (const auto& entry : principalCategories) {
        if (entry.second.size() > 1) {
            // adicionar o nome do ator principal ao vetor
            for (const auto& principal : titlePrincipals) {
                if (principal.nconst == entry.first) {
                    result.push_back(principal.primaryName);
                    break;
                }
            }
        }
    }

    sort(result.begin(), result.end());

    return result;
}
 
//PERGUNTA 5:

int TVSeriesAPP::principalInMultipleGenres(vector<string> vGenres) {

    if (vGenres.empty()) {
        return 0;
    }
    unordered_set<string> uniquePrincipals; // unique atores principais

    // iterar sobre todas as series
    for (const auto& title : titleBasics) {
        // verificar se tem todos os generos do vetor de generos
        bool hasAllGenres = true;
        for (const auto& genre : vGenres) {
            if (find(title.genres.begin(), title.genres.end(), genre) == title.genres.end()) {
                // se não tiver os generos todos break
                hasAllGenres = false;
                break;
            }
        }

        // se a serie tem todos os generos, adicionar os atores principais ao set
        if (hasAllGenres) {
            for (const auto& episode : titleEpisodes) {
                if (episode.parentTconst == title.tconst) {
                    // ver os atores para cada episodio
                    for (const auto& principal : titlePrincipals) {
                        if (principal.tconst == episode.tconst) {
                            // Add the principal to the unordered set
                            uniquePrincipals.insert(principal.primaryName);
                        }
                    }
                }
            }
        }
    }

    // Return the number of unique principals
    return uniquePrincipals.size();
}

//PERGUNTA 6: 
vector<string> TVSeriesAPP::principalsInAllEpisodes(const string& seriesTconst) const {
    vector<string> result; // atores em todos os episodios
    unordered_set<string> principals; // set auxiliar para guardar os atores

    // encontrar todos os episodios da serie
    vector<string> episodes;
    for (const auto& episode : titleEpisodes) {
        if (episode.parentTconst == seriesTconst) {
            episodes.push_back(episode.tconst);
        }
    }

    if (episodes.empty()) {
        return result;
    }

    // encontrar os atores do primeiro episodio
    for (const auto& principal : titlePrincipals) {
        if (principal.tconst == episodes[0]) {
            principals.insert(principal.primaryName);
        }
    }

    // ver o resto dos episodios
    for (size_t i = 1; i < episodes.size(); ++i) {
        unordered_set<string> currentPrincipals; // guardar os atores do episodio atual
        // encontrar os atores do episodio atual
        for (const auto& principal : titlePrincipals) {
            if (principal.tconst == episodes[i]) { // se o ator está no episodio
                currentPrincipals.insert(principal.primaryName); // adicionar o ator ao set
            }
        }
        vector<string> commonPrincipals; // atores comuns a todos os episodios
        for (const auto& principal : principals) { // ver os atores do set
            if (currentPrincipals.count(principal)) { // se o ator está no episodio atual
                commonPrincipals.push_back(principal); // adicionar o ator ao vetor
            }
        }

        principals.clear(); // limpar o set
        for (const auto& principal : commonPrincipals) { // adicionar os atores comuns ao set
            principals.insert(principal); // adicionar o ator ao set
        }
    }

    // converter o set em vector e ordenar 
    result.assign(principals.begin(), principals.end());
    sort(result.begin(), result.end());

    return result;
}

