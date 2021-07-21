#include <iostream>
#include <chrono>
#include <fstream>
#include <ctime>
#include <random>

// АиСД-2, 2021, задание 5
// Ибрагимов Константин, БПИ192
// Среда разработки CLion
// Сделано: всё
// Не сделано: ничего

using namespace std;

long long bubbleSort(int *numbers, int arraySize) {
    auto startCount = std::chrono::steady_clock::now();
    for (int i = 0; i < arraySize - 1; ++i) {
        int j;
        for (j = 0; j < arraySize - i - 1; ++j) {
            if (numbers[j] > numbers[j + 1]) {
                swap(numbers[j], numbers[j + 1]);
            }
        }
    }
    return (std::chrono::steady_clock::now() - startCount).count();
}

long long selectionSort(int *numbers, int arraySize) {
    auto startCount = std::chrono::steady_clock::now();
    int minIndex;
    for (int i = 0; i < arraySize - 1; ++i) {
        minIndex = i;
        for (int j = i + 1; j < arraySize; ++j)
            if (numbers[j] < numbers[minIndex]) {
                minIndex = j;
            }
        if (minIndex != i) {
            swap(numbers[minIndex], numbers[i]);
        }
    }
    return (std::chrono::steady_clock::now() - startCount).count();
}

long long binaryInsertionSort(int *numbers, int arraySize) {
    auto startCount = std::chrono::steady_clock::now();
    for (int i = 1; i < arraySize; ++i) {
        int temp = numbers[i];
        int left = 0, right = i - 1;
        while (left <= right) {
            int middle = (left + right) / 2;
            if (temp < numbers[middle])
                right = middle - 1;
            else
                left = middle + 1;
        }
        for (int j = i; j > left; --j) {
            numbers[j] = numbers[j - 1];
        }
        numbers[left] = temp;
    }
    return (std::chrono::steady_clock::now() - startCount).count();
}

union Un {
    unsigned int num;
    unsigned char arr[4];
};

long long radixSort(int *numbers, int arraySize) {
    auto startCount = std::chrono::steady_clock::now();
    Un uni{};
    int final[arraySize];
    for (int i = 0; i < 4; i++) {
        int quantity[256];
        for (int j = 0; j <= 255; ++j)
            quantity[j] = 0;
        for (int j = 0; j < arraySize; ++j) {
            uni.num = numbers[j];
            quantity[uni.arr[i]]++;
        }
        int count = 0;
        for (int &j : quantity) {
            int temp = j;
            j = count;
            count += temp;
        }
        for (int j = 0; j < arraySize; ++j) {
            uni.num = numbers[j];
            final[quantity[uni.arr[i]]] = numbers[j];
            quantity[uni.arr[i]]++;
        }
        for (int j = 0; j < arraySize; ++j) {
            numbers[j] = final[j];
        }
    }
    return (std::chrono::steady_clock::now() - startCount).count();
}

long long countingSort(int *numbers, int arraySize) {
    auto startCount = std::chrono::steady_clock::now();
    int min, max;
    max = min = numbers[0];
    for (int i = 1; i < arraySize; i++) {
        if (numbers[i] < min) {
            min = numbers[i];
        }
        if (numbers[i] > max) {
            max = numbers[i];
        }
    }

    int quantity[max - min + 1];
    int quantityN = sizeof(quantity) / sizeof(quantity[0]);
    int final[arraySize];

    for (int i = 0; i < quantityN; i++)
        quantity[i] = 0;

    for (int i = 0; i < arraySize; i++) {
        quantity[numbers[i] - min]++;
    }

    for (int i = 1; i < quantityN; ++i) {
        quantity[i] += quantity[i - 1];
    }

    for (int i = arraySize - 1; i >= 0; --i) {
        final[quantity[numbers[i] - min] - 1] = numbers[i];
        quantity[numbers[i] - min]--;
    }

    for (int i = 0; i <= arraySize - 1; ++i) {
        numbers[i] = final[i];
    }
    return (std::chrono::steady_clock::now() - startCount).count();
}

void heapify(int *numbers, int arraySize, int ind) {
    int max = ind;
    int left = 2 * ind + 1;
    int right = 2 * ind + 2;
    if (left < arraySize && numbers[left] > numbers[max])
        max = left;
    if (right < arraySize && numbers[right] > numbers[max])
        max = right;
    if (max != ind) {
        swap(numbers[ind], numbers[max]);
        if (!(right >= arraySize || (2 * max + 1) >= arraySize)) {
            heapify(numbers, arraySize, max);
        }
    }
}

long long heapSort(int *numbers, int arraySize) {
    auto startCount = std::chrono::steady_clock::now();
    for (int i = (arraySize / 2) - 1; i >= 0; --i)
        heapify(numbers, arraySize, i);
    for (int i = arraySize - 1; i > 0; i--) {
        swap(numbers[0], numbers[i]);
        heapify(numbers, i, 0);
    }
    return (std::chrono::steady_clock::now() - startCount).count();
}

void quickSort(int *numbers, int left, int right) {
    int pivot = numbers[left + (right - left) / 2];
    int i = left, j = right;
    while (i <= j) {
        while (numbers[i] < pivot)
            i++;
        while (numbers[j] > pivot)
            j--;
        if (i <= j) {
            swap(numbers[i], numbers[j]);
            i++, j--;
        }
    }
    if (left < j)
        quickSort(numbers, left, j);
    if (right > i)
        quickSort(numbers, i, right);
}

void merge(int *numbers, int left, int middle, int right) {
    int count1 = middle - left + 1;
    int count2 = right - middle;

    int leftTemp[count1], rightTemp[count2];
    for (int i = 0; i < count1; i++)
        leftTemp[i] = numbers[left + i];
    for (int j = 0; j < count2; j++)
        rightTemp[j] = numbers[middle + j + 1];

    int i = 0, j = 0, k = left;

    while (i < count1 && j < count2) {
        if (leftTemp[i] <= rightTemp[j]) {
            numbers[k] = leftTemp[i];
            i++;
        } else {
            numbers[k] = rightTemp[j];
            j++;
        }
        k++;
    }
    while (i < count1) {
        numbers[k] = leftTemp[i];
        i++, k++;
    }
    while (j < count2) {
        numbers[k] = rightTemp[j];
        j++, k++;
    }
}

void mergeSort(int *numbers, int left, int right) {
    if (left < right) {
        int middle = left + (right - left) / 2;
        mergeSort(numbers, left, middle);
        mergeSort(numbers, middle + 1, right);
        merge(numbers, left, middle, right);
    }
}

int *copy(const int *numbers, int arraySize) {
    int *newArr = new int[arraySize];
    for (int i = 0; i < arraySize; ++i) {
        newArr[i] = numbers[i];
    }
    return newArr;
}

/**
 * Метод для генерации тестовых массивов
 * @param arraySize
 * @param step
 * @param type
 */
void createTests(int arraySize, int step, int type) {
    srand(time(nullptr));
    ofstream stream(R"(D:\C++\SortCheck_4\input\test_)" + to_string(arraySize) + "_" +
                    to_string(type) + ".txt",
                    ios::app);
    if (type == 0) {
        for (int i = 0; i < arraySize; ++i) {
            stream << rand() % 6 << " ";
        }
    } else if (type == 1) {
        for (int i = 0; i < arraySize; ++i) {
            stream << rand() % 4001 << " ";
        }
    } else if (type == 2) {
        int *arr = new int[arraySize];
        for (int i = 0; i < arraySize; ++i) {
            arr[i] = i;
        }
        for (int i = 0; i < arraySize / step + 1; ++i) {
            swap(arr[rand() % arraySize], arr[rand() % arraySize]);
        }
        for (int i = 0; i < arraySize; ++i) {
            stream << arr[i] << " ";
        }
        delete[] arr;
    } else if (type == 3) {
        for (int i = arraySize - 1; i >= 0; --i) {
            stream << i << " ";
        }
    }
    stream.close();
}

class pipeLine {
public:
    int *values;
};

/**
 * Метод для создания csv файла
 * @param numbers
 * @param arraySize
 * @param step
 * @param type
 * @param index
 */
void createCSV(int *numbers, int arraySize, int step, int type, int index) {
    auto *arrays = new pipeLine[8];
    for (int i = 0; i < 8; ++i)
        arrays[i].values = copy(numbers, arraySize);

    long long bubbleTime = bubbleSort(arrays[0].values, arraySize);
    long long binaryInsertTime = binaryInsertionSort(arrays[1].values, arraySize);
    long long radixTime = radixSort(arrays[2].values, arraySize);
    long long selectionTime = selectionSort(arrays[3].values, arraySize);
    long long heapTime = heapSort(arrays[4].values, arraySize);
    long long countingTime = countingSort(arrays[5].values, arraySize);
    auto timer = std::chrono::steady_clock::now();
    mergeSort(arrays[6].values, 0, arraySize);
    auto mergeTime = (std::chrono::steady_clock::now() - timer).count();
    timer = std::chrono::steady_clock::now();
    quickSort(arrays[7].values, 0, arraySize);
    auto quickTime = (std::chrono::steady_clock::now() - timer).count();

    for(int i = 0; i < 8; ++i)
        delete[] arrays[i].values;
    delete[] arrays;

    if (index > 2) {
        ofstream stream(R"(D:\C++\SortCheck_4\output\output.csv)", ios::app);
        stream << "test" << "_" << arraySize << "_" << type << ";";
        stream << arraySize << ";" << step << ";" << type << ";" << bubbleTime << ";" << binaryInsertTime
               << ";";
        stream << radixTime << ";" << selectionTime << ";" << heapTime << ";" << countingTime << ";" << mergeTime
               << ";";
        stream << quickTime << "\n";
        stream.close();
    }
}
/**
 * Метод для чтения тестов из файлов
 * @param low
 * @param high
 * @param step
 * @param type
 * @param index
 */
void process(int low, int high, int step, int type, int index) {
    for (int arraySize = low; arraySize <= high; arraySize += step) {
        ifstream in(R"(D:\C++\SortCheck_4\input\test_)" + to_string(high) + "_" +
                    to_string(type) + ".txt", ios::app);
        int *arr = new int[arraySize];
        for (int i = 0; i < arraySize; ++i) {
            in >> arr[i];
        }
        in.close();

        createCSV(arr, arraySize, step, type, index);
        delete[] arr;
    }
}

int main() {
    /* for (int i = 0; i < 4; ++i) {
         createTests(300, 50, i);
         createTests(4100, 1000, i);
     }*/
    ofstream stream1(R"(D:\C++\SortCheck_4\output\output.csv)", ios::app);
    stream1 << "test;size;step;type;bubble;binaryInsert;radix;selection;heap;counting;merge;quick\n";
    stream1.close();
    for (int i = 0; i < 13; ++i) {
        for (int j = 0; j < 4; ++j) {
            process(50, 300, 50, j, i);
            process(100, 4100, 1000, j, i);
        }
        if (i > 2) {
            ofstream stream2(R"(D:\C++\SortCheck_4\output\output.csv)", ios::app);
            stream2 << "FINISH;0;0;0;0;0;0;0;0;0;0;0\n";
            stream2.close();
        }
    }
    return 0;
}