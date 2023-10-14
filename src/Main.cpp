#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <set>

struct BinaryValue
{
    std::string value;
    int ones;
    int decimal;
    bool ticked = false;
};

std::vector<std::string> splitValues(const std::string& str)
{
    std::vector<std::string> returnValues;
    size_t previous = 0;
    size_t position = str.find(" ");
    std::string splice;

    while (position != std::string::npos)
    {
        splice = str.substr(previous, position - previous);
        returnValues.push_back(splice);
        previous = position + 1;
        position = str.find(" ", position + 1);
    }
    splice = str.substr(previous);

    if (splice == "") return returnValues;

    returnValues.push_back(splice);


    return returnValues;
}

std::vector<BinaryValue> convertToBinary(std::string str, int bits)
{
    std::string newString;
    std::vector<BinaryValue> returnStrings;

    std::vector<std::string> splicedString = splitValues(str);

    if (splicedString.size() == 0) return returnStrings;

    for (std::string str : splicedString)
    {
        newString = "";
        int value = std::stoi(str);
        int ones = 0;

        for (int i = 0; i < bits; i++)
        {
            int bit = ((value) >> (bits - 1 - i)) & 0x1;
            if (bit == 1)
            {
                ones++;
                newString += '1';
            }
            else
            {
                newString += '0';
            }
        }

        BinaryValue bv;
        bv.value = newString;
        bv.ones = ones;
        bv.decimal = value;
        returnStrings.push_back(bv);
    }

    return returnStrings;
}

std::vector<std::vector<BinaryValue>> group(std::vector<BinaryValue> minterms, std::vector<BinaryValue> dontcares, int bits)
{
    std::vector<std::vector<BinaryValue>> groups;
    for (int i = 0; i < bits + 1; i++)
        groups.push_back({});

    for (BinaryValue s : minterms)
    {
        groups.at(s.ones).push_back(s);
    }

    for (BinaryValue s : dontcares)
    {
        groups.at(s.ones).push_back(s);
    }

    return groups;
}

bool differByOne(BinaryValue v1, BinaryValue v2)
{
    bool different = false;
    for (size_t i = 0; i < v1.value.size(); i++)
    {
        if (v1.value.at(i) == v2.value.at(i))
        {
            continue;
        }
        else
        {
            if (v1.value.at(i) == '-') return false;
            if (different) return false;

            different = true;
        }
    }

    return different;
}

BinaryValue difference(BinaryValue v1, BinaryValue v2)
{
    BinaryValue returnValue;
    std::string newValue = "";
    for (size_t i = 0; i < v1.value.size(); i++)
    {
        if (v1.value.at(i) == v2.value.at(i))
        {
            newValue += v1.value.at(i);
            continue;
        }
        else
        {
            newValue += '-';
        }
    }
    returnValue.value = newValue;
    returnValue.ones = std::min(v1.ones, v2.ones);

    return returnValue;
}

std::vector<BinaryValue> combineGroups(const std::vector<std::vector<BinaryValue>>& terms, int bits)
{
    std::vector<BinaryValue> returnValues;

    std::vector<std::vector<BinaryValue>> currentGroups = terms;
    std::vector<std::vector<BinaryValue>> nextGroups;

    for (int i = 0; i < bits + 1; i++)
        nextGroups.push_back({});

    bool allStarred = false;
    while (!allStarred)
    {
        allStarred = true;
        for (int i = 0; i < bits; i++)
        {
            for (BinaryValue& v1 : currentGroups.at(i))
            {
                for (BinaryValue& v2 : currentGroups.at(i + 1))
                {
                    if (differByOne(v1, v2))
                    {
                        BinaryValue diff = difference(v1, v2);
                        nextGroups.at(diff.ones).push_back(diff);
                        v1.ticked = true;
                        v2.ticked = true;
                        allStarred = false;
                    }
                }
            }
        }

        for (auto& vec : currentGroups)
        {
            for (BinaryValue& v : vec)
            {
                if (!v.ticked)
                {
                    returnValues.push_back(v);
                }
                v.ticked = false;
            }
        }

        currentGroups = nextGroups;
        nextGroups.clear();

        for (int i = 0; i < bits + 1; i++)
            nextGroups.push_back({});
    }

    return returnValues;
}

std::vector<BinaryValue> removeDuplicates(std::vector<BinaryValue>& values)
{
    std::set<std::string> foundValues;
    std::vector<BinaryValue> returnValues;

    for (BinaryValue v : values)
    {
        foundValues.insert(v.value);
    }

    for (auto it = foundValues.begin(); it != foundValues.end(); it++)
    {
        BinaryValue v;
        v.value = *it;
        returnValues.push_back(v);
    }

    return returnValues;
}

void convertToBinary(BinaryValue value, std::vector<BinaryValue>& values)
{
    bool hyphen = false;
    std::string valueStr = value.value;
    for (size_t i = 0; i < valueStr.size(); i++)
    {
        if (valueStr[i] == '-')
        {
            std::string alternative = valueStr;
            alternative[i] = '0';
            BinaryValue temp = value;
            temp.value = alternative;
            convertToBinary(temp, values);

            alternative[i] = '1';
            temp = value;
            temp.value = alternative;
            temp.ones++;
            convertToBinary(temp, values);

            hyphen = true;
                break;
        }
    }
    if (hyphen == false)
    {
        values.push_back(value);
        return;
    }
}

int convertToDenary(BinaryValue value)
{
    int i = 0;
    for (char c : value.value)
    {
        i = i << 1;
        i |= c - '0';
    }

    return i;
}

bool exists(std::map<int, std::vector<int>> map, int key)
{
    return (map.find(key) != map.end());
}

bool exists(std::map<int, int> map, int key)
{
    return (map.find(key) != map.end());
}

/* Needs to be reworked to explore all paths when multiple values have the same number of outcomes
 * and to choose the outcome with the fewest outputs
 */
std::vector<BinaryValue> primeImplicantChart(std::vector<BinaryValue> values, std::vector<BinaryValue> minterms)
{
    std::map<int, std::vector<int>> value_denary;
    std::map<int, std::vector<int>> denary_value;

    std::map<int, int> denary_index;

    for (size_t i = 0; i < minterms.size(); i++)
    {
        denary_index[minterms.at(i).decimal] = i;
    }

    for (size_t i = 0; i < values.size(); i++)
    {
        BinaryValue v = values.at(i);
        std::vector<BinaryValue> newValues;
        convertToBinary(v, newValues);

        for (size_t j = 0; j < newValues.size(); j++)
        {
            int denary = convertToDenary(newValues.at(j));

            if (exists(denary_index, denary))
            {
                denary_value[denary].push_back(i);
                value_denary[i].push_back(denary);
            }

        }
    }

    std::vector<BinaryValue> returnValues;

    std::vector<int> toRemove;
    for (auto it = denary_value.begin(); it != denary_value.end(); it++)
    {
        std::cout << it->first << " " << it->second.size();
        if (it->second.size() == 1)
        {
            std::cout << " X";
            toRemove.push_back(it->second.at(0));
            returnValues.push_back(values.at(it->second.at(0)));
        }
        std::cout << "\n";
    }


    for (int i : toRemove)
    {
        std::cout << values.at(i).value << "\n";
        std::vector<int> denary = value_denary[i];

        for (int d : denary)
        {
            std::vector<int> values = denary_value[d];
            for (int v : values)
            {
                auto pos = std::find(value_denary[v].begin(), value_denary[v].end(), d);
                value_denary[v].erase(pos);
            }
            denary_value.erase(d);
        }
    }
    std::cout << "\n";

    bool empty = false;
    while (!empty)
    {
        int value;
        size_t highest = 0;
        for (auto it = value_denary.begin(); it != value_denary.end();)
        {
            if (it->second.size() > highest)
            {
                highest = it->second.size();
                value = it->first;
            }
            it++;
        }

        returnValues.push_back(values.at(value));
        std::vector<int> denary = value_denary[value];

        for (size_t i = 0; i < denary.size(); i++)
        {
            int index = denary[i];
            std::vector<int> newValue = denary_value[index];
            for (size_t j = 0; j < newValue.size(); j++)
            {
                int index2 = newValue[j];
                auto pos = std::find(value_denary[index2].begin(), value_denary[index2].end(), index);
                if (pos != value_denary[index2].end())
                    value_denary[index2].erase(pos);
            }
            denary_value.erase(index);
        }
        value_denary.erase(value);

        empty = true;
        for (auto& v : value_denary)
        {
            if (v.second.size() != 0)
            {
                empty = false;
                break;
            }
        }
    }

    returnValues = removeDuplicates(returnValues);

    return returnValues;
}

std::string convertToSOP(std::vector<std::string> variables, std::vector<BinaryValue> values)
{
    std::string returnString = "";
    size_t bits = variables.size();
    size_t totalIndex = 0;
    for (BinaryValue v : values)
    {
        std::string str = v.value;
        size_t index = 0;
        bool started = false;
        for (size_t i = 0; i < str.length(); i++)
        {
            char c = str[i];

            if (c == '0')
            {
                returnString += "¬" + variables.at(index);
                started = true;
            }
            else if (c == '1')
            {
                returnString += variables.at(index);
                started = true;
            }
            index++;

            if (index != bits)
            {
                char c1 = str[i + 1];
                if (c1 != '-' && started)
                    returnString += ".";
            }
        }

        if (totalIndex != values.size() - 1)
        {
            returnString += " + ";
        }
        totalIndex++;
    }

    return returnString;
}

int main()
{
    /* std::cout << "Please enter the name of the variables, space seperated\n"; */
    /* std::string variableNames; */
    /* std::getline(std::cin, variableNames); */
    /**/
    /* std::cout << "Please enter the denary values of the minterms, space seperated\n"; */
    /* std::string minterms; */
    /* std::getline(std::cin, minterms); */
    /**/
    /* std::cout << "Please enter the denary values of the don't care's\n"; */
    /* std::string dontcares; */
    /* std::getline(std::cin, dontcares); */

    std::string variableNames = "A B C D E";
    std::string minterms = "0 1 2 3 6 8 9 10 11 17 20 21 23 25 28 30 31";
    std::string dontcares = "";

    std::vector<std::string> variables = splitValues(variableNames);
    int bits = variables.size();
    std::vector<BinaryValue> mintermBinary = convertToBinary(minterms, bits);
    std::vector<BinaryValue> dontcaresBinary = convertToBinary(dontcares, bits);

    std::vector<std::vector<BinaryValue>> groups = group(mintermBinary, dontcaresBinary, bits);
    std::vector<BinaryValue> starred = combineGroups(groups, bits);
    starred = removeDuplicates(starred);

    for (BinaryValue v : starred)
    {
        std::cout << v.value << "\n";
    }

    std::vector<BinaryValue> results = primeImplicantChart(starred, mintermBinary);

    for (BinaryValue v : results)
    {
        std::cout << v.value << "\n";
    }

    std::string output = convertToSOP(variables, results);
    std::cout << output << "\n";

    return 0;
}

