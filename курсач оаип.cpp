#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstdio>
#include <io.h>
#include <string>
#include <ctime>
#include <algorithm>
using namespace std;

struct St
{
    char FIO[50];
    char Adress[50];
    char number[14];
    char HospDate[11]; // Формат: ДД.ММ.ГГГГ
    char OutDate[11];  // Формат: ДД.ММ.ГГГГ (ключ сортировки)

    // Операторы сравнения для сортировки по дате выписки
    bool operator<(const St& other) const {
        return strcmp(OutDate, other.OutDate) < 0;
    }
    bool operator>(const St& other) const {
        return strcmp(OutDate, other.OutDate) > 0;
    }
    bool operator==(const St& other) const {
        return strcmp(OutDate, other.OutDate) == 0;
    }
};

// Прототипы функций
void Create(FILE* file);
void Show(FILE* file);
void InfoPatients(St* s);
void QuickSort(St* arr, int left, int right);
void MergeSort(St* arr, int l, int r);
void Merge(St* arr, int l, int m, int r);
int BinarySearch(FILE* file, const char* targetDate);
int InterpolationSearch(FILE* file, const char* targetDate);
void SortAndSave(FILE* srcFile, FILE* destFile, int sortType);
void SearchMenu(FILE* file);
void ShowPatientsByDischargeMonth(FILE* file);
void SortPatientsByFIO(St* patients, int count);

void Menu() {
    FILE* file = fopen("hospital.txt", "wb");
    FILE* sortedFile = fopen("hospital_sorted.txt", "wb");
    fclose(file);
    fclose(sortedFile);

    bool menu = true;
    while (menu) {
        cout << "\nМеню:\n";
        cout << "1. Добавить пациента\n";
        cout << "2. Просмотреть всех пациентов\n";
        cout << "3. Отсортировать данные по дате выписки (QuickSort)\n";
        cout << "4. Отсортировать данные по дате выписки (MergeSort)\n";
        cout << "5. Поиск пациента по дате выписки\n";
        cout << "6. Вывести пациентов, выписанных за определённый месяц (с сортировкой по ФИО)\n";
        cout << "9. Выйти из программы\n";
        cout << "Выберите пункт: ";

        int n;
        cin >> n;
        cin.ignore();
        switch (n) {
        case 1: Create(file); break;
        case 2:
            cout << "\nИсходные данные:\n";
            Show(file);
            cout << "\nОтсортированные данные по дате выписки:\n";
            Show(sortedFile);
            break;
        case 3:
            SortAndSave(file, sortedFile, 1);
            cout << "Данные отсортированы по дате выписки методом QuickSort\n";
            break;
        case 4:
            SortAndSave(file, sortedFile, 2);
            cout << "Данные отсортированы по дате выписки методом MergeSort\n";
            break;
        case 5: SearchMenu(sortedFile); break;
        case 6: ShowPatientsByDischargeMonth(file); break;
        case 9: menu = false; break;
        default: cout << "Такого пункта нет\n";
        }
    }
}

int main() {
    system("chcp 1251");
    Menu();
    return 0;
}

void Create(FILE* file) {
    file = fopen("hospital.txt", "ab");
    St s;

    cout << "Введите ФИО: ";
    cin.getline(s.FIO, 50);

    cout << "Введите адрес: ";
    cin.getline(s.Adress, 50);

    cout << "Введите номер телефона: ";
    cin.getline(s.number, 14);

    cout << "Введите дату госпитализации (ДД.ММ.ГГГГ): ";
    cin.getline(s.HospDate, 11);

    cout << "Введите дату выписки (ДД.ММ.ГГГГ): ";
    cin.getline(s.OutDate, 11);

    fwrite(&s, sizeof(St), 1, file);
    fclose(file);
    cout << "Пациент успешно добавлен!\n";
}

void Show(FILE* file) {
    file = fopen("hospital.txt", "rb");
    if (!file) {
        cout << "Файл пуст или не существует\n";
        return;
    }

    St s;
    while (fread(&s, sizeof(St), 1, file) == 1) {
        InfoPatients(&s);
        cout << "----------------------------\n";
    }
    fclose(file);
}

void InfoPatients(St* s) {
    cout << "ФИО: " << s->FIO << "\nАдрес: " << s->Adress
        << "\nНомер: " << s->number << "\nДата госпитализации: "
        << s->HospDate << "\nДата выписки: " << s->OutDate << endl;
}

void QuickSort(St* arr, int left, int right) {
    if (left >= right) return;

    int i = left, j = right;
    St pivot = arr[(left + right) / 2];

    while (i <= j) {
        while (arr[i] < pivot) i++;
        while (arr[j] > pivot) j--;

        if (i <= j) {
            swap(arr[i], arr[j]);
            i++;
            j--;
        }
    }

    if (left < j) QuickSort(arr, left, j);
    if (i < right) QuickSort(arr, i, right);
}

void Merge(St* arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    St* L = new St[n1];
    St* R = new St[n2];

    for (int i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;

    while (i < n1 && j < n2) {
        if (L[i] < R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }

    delete[] L;
    delete[] R;
}

void MergeSort(St* arr, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        MergeSort(arr, l, m);
        MergeSort(arr, m + 1, r);
        Merge(arr, l, m, r);
    }
}

int BinarySearch(FILE* file, const char* targetDate) {
    file = fopen("hospital_sorted.txt", "rb");
    if (!file) return -1;

    int low = 0;
    int high = _filelength(_fileno(file)) / sizeof(St) - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;
        fseek(file, mid * sizeof(St), SEEK_SET);

        St s;
        fread(&s, sizeof(St), 1, file);

        int cmp = strcmp(s.OutDate, targetDate);

        if (cmp == 0) {
            fclose(file);
            return mid;
        }
        else if (cmp < 0) {
            low = mid + 1;
        }
        else {
            high = mid - 1;
        }
    }

    fclose(file);
    return -1;
}

int InterpolationSearch(FILE* file, const char* targetDate) {
    file = fopen("hospital_sorted.txt", "rb");
    if (!file) return -1;

    int low = 0;
    int high = _filelength(_fileno(file)) / sizeof(St) - 1;

    while (low <= high) {
        // Получаем даты для интерполяции
        fseek(file, low * sizeof(St), SEEK_SET);
        St sLow;
        fread(&sLow, sizeof(St), 1, file);

        fseek(file, high * sizeof(St), SEEK_SET);
        St sHigh;
        fread(&sHigh, sizeof(St), 1, file);

        // Если все даты одинаковые
        if (strcmp(sLow.OutDate, sHigh.OutDate) == 0) {
            if (strcmp(sLow.OutDate, targetDate) == 0) {
                fclose(file);
                return low;
            }
            else {
                fclose(file);
                return -1;
            }
        }

        // Преобразуем даты в числовые значения для интерполяции
        int lowDate, highDate, target;
        sscanf(sLow.OutDate + 6, "%4d", &lowDate);
        sscanf(sLow.OutDate + 3, "%2d", &lowDate);
        sscanf(sLow.OutDate, "%2d", &lowDate);

        sscanf(sHigh.OutDate + 6, "%4d", &highDate);
        sscanf(sHigh.OutDate + 3, "%2d", &highDate);
        sscanf(sHigh.OutDate, "%2d", &highDate);

        sscanf(targetDate + 6, "%4d", &target);
        sscanf(targetDate + 3, "%2d", &target);
        sscanf(targetDate, "%2d", &target);

        // Интерполяционная формула
        int pos = low + ((target - lowDate) * (high - low)) / (highDate - lowDate);

        if (pos < low || pos > high) {
            fclose(file);
            return -1;
        }

        fseek(file, pos * sizeof(St), SEEK_SET);
        St s;
        fread(&s, sizeof(St), 1, file);

        int cmp = strcmp(s.OutDate, targetDate);

        if (cmp == 0) {
            fclose(file);
            return pos;
        }
        else if (cmp < 0) {
            low = pos + 1;
        }
        else {
            high = pos - 1;
        }
    }

    fclose(file);
    return -1;
}

void SortAndSave(FILE* srcFile, FILE* destFile, int sortType) {
    srcFile = fopen("hospital.txt", "rb");
    if (!srcFile) return;

    int size = _filelength(_fileno(srcFile)) / sizeof(St);
    St* arr = new St[size];

    for (int i = 0; i < size; i++) {
        fread(&arr[i], sizeof(St), 1, srcFile);
    }
    fclose(srcFile);

    if (sortType == 1) {
        QuickSort(arr, 0, size - 1);
    }
    else {
        MergeSort(arr, 0, size - 1);
    }

    destFile = fopen("hospital_sorted.txt", "wb");
    for (int i = 0; i < size; i++) {
        fwrite(&arr[i], sizeof(St), 1, destFile);
    }
    fclose(destFile);

    delete[] arr;
}

void SearchMenu(FILE* file) {
    cout << "\nМеню поиска по дате выписки:\n";
    cout << "1. Бинарный поиск\n";
    cout << "2. Интерполяционный поиск\n";
    cout << "Выберите метод поиска: ";

    int choice;
    cin >> choice;
    cin.ignore();

    cout << "Введите дату выписки для поиска (ДД.ММ.ГГГГ): ";
    char targetDate[11];
    cin.getline(targetDate, 11);

    int result = -1;
    clock_t start, end;
    double time_taken;

    switch (choice) {
    case 1:
        start = clock();
        result = BinarySearch(file, targetDate);
        end = clock();
        time_taken = double(end - start) / CLOCKS_PER_SEC;
        cout << "Время выполнения бинарного поиска: " << time_taken << " сек\n";
        break;
    case 2:
        start = clock();
        result = InterpolationSearch(file, targetDate);
        end = clock();
        time_taken = double(end - start) / CLOCKS_PER_SEC;
        cout << "Время выполнения интерполяционного поиска: " << time_taken << " сек\n";
        break;
    default:
        cout << "Неверный выбор\n";
        return;
    }

    if (result != -1) {
        cout << "\nНайден пациент:\n";
        file = fopen("hospital_sorted.txt", "rb");
        fseek(file, result * sizeof(St), SEEK_SET);
        St s;
        fread(&s, sizeof(St), 1, file);
        InfoPatients(&s);
        fclose(file);
    }
    else {
        cout << "Пациент с такой датой выписки не найден\n";
    }
}

void SortPatientsByFIO(St* patients, int count) {
    // Сортировка вставками по ФИО
    for (int i = 1; i < count; i++) {
        St key = patients[i];
        int j = i - 1;

        while (j >= 0 && strcmp(patients[j].FIO, key.FIO) > 0) {
            patients[j + 1] = patients[j];
            j--;
        }
        patients[j + 1] = key;
    }
}

void ShowPatientsByDischargeMonth(FILE* file) {
    file = fopen("hospital.txt", "rb");
    if (!file) {
        cout << "Файл пуст или не существует\n";
        return;
    }

    cout << "Введите месяц выписки (число от 1 до 12): ";
    int month;
    cin >> month;
    cin.ignore();

    if (month < 1 || month > 12) {
        cout << "Некорректный месяц!\n";
        fclose(file);
        return;
    }

    // Определяем размер файла
    fseek(file, 0, SEEK_END);
    int fileSize = ftell(file);
    rewind(file);

    int maxPatients = fileSize / sizeof(St);
    St* patients = new St[maxPatients];
    int count = 0;

    // Читаем пациентов и фильтруем по месяцу
    St s;
    while (fread(&s, sizeof(St), 1, file) == 1) {
        int dischargeMonth;
        if (sscanf(s.OutDate + 3, "%d", &dischargeMonth) != 1) {
            cout << "Ошибка формата даты у пациента: " << s.FIO << endl;
            continue;
        }

        if (dischargeMonth == month) {
            patients[count++] = s;
        }
    }
    fclose(file);

    if (count == 0) {
        cout << "Нет пациентов, выписанных в указанный месяц\n";
        delete[] patients;
        return;
    }

    // Сортируем пациентов по ФИО для вывода
    SortPatientsByFIO(patients, count);

    // Выводим результат
    cout << "\nПациенты, выписанные в месяце " << month << " (в алфавитном порядке):\n";
    for (int i = 0; i < count; i++) {
        InfoPatients(&patients[i]);
        cout << "----------------------------\n";
    }

    delete[] patients;
}