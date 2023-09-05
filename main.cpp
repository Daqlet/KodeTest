#include <bits/stdc++.h>
using namespace std;

struct Object {
    string name;
    double x;
    double y;
    string type;
    double createdAt;
};

const vector<string> DistToString = {"До 100 ед.", "До 1000 ед.", "До 10000 ед.", "Слишком далеко"};
const vector<string> TimeToString = {"Сегодня", "Вчера", "На этой неделе", "В этом месяце", "В этом году", "Ранее"}; 

vector<string> TypeToString = {"Разное"};
unordered_map<string, pair<int, int>> Types;
const int N = 1;

istream& operator>>(istream& in, Object& object) {
    return in >> object.name >> object.x >> object.y >> object.type >> object.createdAt;
}

ostream& operator<<(ostream& out, const Object& object) {
    return out << fixed << object.name << " " << object.x << " " << object.y << " " << object.type << " " << object.createdAt;
}

template <typename T>
void GroupBy(list<Object>& objects, int groupCount, int (*getGroup)(const Object&), T (*groupToString)(int), string fileName) {
    vector<list<Object*>> groups(groupCount);
    for (auto& obj: objects) {
        cout << getGroup(obj) << endl;
        groups[getGroup(obj)].push_back(&obj);
    }
    ofstream out(fileName);
    for (int i = 0; i < groups.size(); ++i) {
        if (groups[i].empty()) {
            continue;
        }
        out << groupToString(i) << endl;
        for (auto* obj: groups[i]) {
            out << *obj << endl;
        }
    }
}

int GetGroupByDistance(const Object& obj) {
    double distance = sqrt(obj.x * obj.x + obj.y * obj.y);  
    if (distance < 100) {
        return 0;
    } else if (distance < 1000) {
        return 1;
    } else if (distance < 10000) {
        return 2;
    } else {
        return 3;
    }
}

const string& DistanceGroupToString(int group) {
    return DistToString[group];
}

void GroupByDistances(list<Object>& objects) {
    GroupBy(objects, 4, GetGroupByDistance, DistanceGroupToString, "objects_group_by_distance.txt");
}

int GetGroupByName(const Object& obj) {
    if (obj.name.empty()) {
        return 66;
    }
    auto first = obj.name[0];
    cout << first << endl;
    if (first >= 'А' && first <= 'п') {
        return first - 'A';
    }
    if (first >= 'р' && first <= 'я') {
        return first - 'р';
    }
    return 66;
}

char NameGroupToString(int group) {
    if (group == 66) {
        return '#';
    }
    if (group < 33) {
        return group + 'А';
    }
    return group + 'а';
}

void GroupByNames(list<Object>& objects) {
    GroupBy(objects, 67, GetGroupByName, NameGroupToString, "objects_group_by_names.txt");
}

int GetGroupByCreatedAt(const Object& obj) {
    time_t now = time(0);
    tm nowTm = *gmtime(&now);
    time_t createdAt = obj.createdAt;
    tm createdTm = *gmtime(&createdAt);
    cout << ctime(&createdAt) << endl;

    if (nowTm.tm_year == createdTm.tm_year) {
        cout << nowTm.tm_yday << " " << createdTm.tm_yday << endl;
        if (nowTm.tm_yday == createdTm.tm_yday) {
            return 0;
        } else if (nowTm.tm_yday == createdTm.tm_yday + 1) {
            return 1;
        } else if (nowTm.tm_yday <= createdTm.tm_yday + 7 && nowTm.tm_wday > createdTm.tm_wday) {
            return 2;
        } else if (nowTm.tm_mon == createdTm.tm_mon) {
            return 3;
        } 
        return 4;
    } else {
        return 5;
    }
}

const string& CreatedAtGroupToString(int group) {
    return TimeToString[group];
}

void GroupByCreatedTimes(list<Object>& objects) {
    GroupBy(objects, 6, GetGroupByCreatedAt, CreatedAtGroupToString, "objects_group_by_created_time.txt");
}

int GetGroupByType(const Object& obj) {
    if (Types[obj.type].second >= N) {
        return Types[obj.type].first;
    }
    return 0;
}

const string& TypeGroupToString(int group) {
    return TypeToString[group];
}

void GroupByTypes(list<Object>& objects) {
    GroupBy(objects, TypeToString.size(), GetGroupByType, TypeGroupToString,  "objects_group_by_type");
}

int main() {
    ifstream in("objects.txt");
    list<Object> objects;

    Object temp;
    while (in >> temp) {
        auto& [typeGroup, count] = Types[temp.type];
        if (count++ == 0) {
            typeGroup = TypeToString.size();
            TypeToString.push_back(temp.type);
        }
        objects.emplace_back(std::move(temp));
    }

    char choice = '#';
    while (choice != '0') {
        cout << "Choose: " << endl;
        cout << "[0] EXIT" << endl;
        cout << "[1] Add an object to the list" << endl;
        cout << "[2] Show the list of objects" << endl;
        cout << "[3] Group the objects by distances" << endl;
        cout << "[4] Group the objects by names" << endl;
        cout << "[5] Group the objects by created times" << endl;
        cout << "[6] Group the objects by types" << endl;
        cin >> choice;
        if (choice == '0') {
            cout << "Bye!" << endl;
        } else if (choice == '1') {
            Object temp;
            cin >> temp;
            auto& [typeGroup, count] = Types[temp.type];
            if (count++ == 0) {
                typeGroup = TypeToString.size();
                TypeToString.push_back(temp.type);
            }
            objects.emplace_back(move(temp));   
        } else if (choice == '2') {
            for (const auto& obj: objects) {
                cout << obj << endl;
            }
        } else if (choice == '3') {
            GroupByDistances(objects);
        } else if (choice == '4') {
            GroupByNames(objects);
        } else if (choice == '5') {
            GroupByCreatedTimes(objects);
        } else if (choice == '6') {
            GroupByTypes(objects);
        }
    }
}

