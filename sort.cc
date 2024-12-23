#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <algorithm>
#include <map>
#include <string>
#include <cmath>
#include <random>

using namespace std;

// interface
void generateTestCases(int = 60, int = 10000);
void writeFile(vector<vector<int>> &);
bool readFile(vector<vector<int>> &);
void printArray(vector<int>);
void printTestCaseSize(vector<vector<int>>);
void test(vector<vector<int>>, string);
// TODO: visualization_sorting

// helper functions
void random_shuffle(vector<int>::iterator, vector<int>::iterator);
void swap(int &a, int &b)
{
    // becareful when a and b are the same
    // this will set both a and b to 0
    if (a == b)
    {
        return;
    }
    a = a ^ b;
    b = a ^ b; // b = (a ^ b) ^ b = a
    a = a ^ b; // a = (a ^ b) ^ b = (now b is a) = a ^ b ^ a = b
}
// heapify
void heapify(vector<int> &array, int n, int i)
{
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && array[left] > array[largest])
    {
        largest = left;
    }

    if (right < n && array[right] > array[largest])
    {
        largest = right;
    }

    if (largest != i)
    {
        swap(array[i], array[largest]);
        heapify(array, n, largest);
    }
}

// sort algorithms
void bubbleSort(vector<int> &);
void selectionSort(vector<int> &);
void insertionSort(vector<int> &);
void mergeSort(vector<int> &);
void quickSort(vector<int> &);
void heapSort(vector<int> &);
void countingSort(vector<int> &);
void radixSort(vector<int> &);
void bucketSort(vector<int> &);
void shellSort(vector<int> &);
void cocktailSort(vector<int> &);
void combSort(vector<int> &);
void gnomeSort(vector<int> &);

int main(int argc, char *argv[])
{
    // seed the random number generator
    srand(time(NULL));

    // generate test cases and write to file if argument is provided
    if (argc == 2 && string(argv[1]) == "gen")
    {
        generateTestCases();
        cout << "Test cases generated: input.txt" << endl;
        return 0;
    } // show test cases if argument is provided
    else if (argc == 2 && string(argv[1]) == "show")
    {
        vector<vector<int>> arraylist;
        if (!readFile(arraylist))
        {
            return 1;
        }
        printTestCaseSize(arraylist);
        return 0;
    } // show help message if argument is provided
    else if (argc == 2 && string(argv[1]) == "help")
    {
        cout << "Usage: ./sort [gen|help|<algo_name>]" << endl;
        cout << "\nOptions:" << endl;
        cout << "gen: generate test cases and write to file" << endl;
        cout << "show: show test cases" << endl;
        cout << "help: show help message" << endl;
        cout << "<algo_name>: sort the test cases with the specified algorithm" << endl;
        cout << "Available algorithms: bubble, selection, insertion, merge, quick, heap, counting, radix, bucket, shell, cocktail, comb, gnome" << endl;
        return 0;
    } // no argument provided is not allowed
    else if (argc == 1)
    {
        // test swap function
        int a = 5, b = 10;
        cout << "Before swap: a = " << a << ", b = " << b << endl;
        swap(a, b);
        cout << "After swap: a = " << a << ", b = " << b << endl;
        cerr << "Error: No argument provided" << endl;
        return 1;
    } // more than 2 arguments are not allowed
    else if (argc > 2)
    {
        cerr << "Error: Too many arguments" << endl;
        return 1;
    } // sort the array if two arguments are provided
    else
    {
        // available commands map to sort functions
        map<string, void (*)(vector<int> &)> sortFunctions = {
            {"bubble", bubbleSort},
            {"selection", selectionSort},
            {"insertion", insertionSort},
            {"merge", mergeSort},
            {"quick", quickSort},
            {"heap", heapSort},
            {"counting", countingSort},
            {"radix", radixSort},
            {"bucket", bucketSort},
            {"shell", shellSort},
            {"cocktail", cocktailSort},
            {"comb", combSort},
            {"gnome", gnomeSort}};

        // user input
        string command = argv[1];

        // check if the command is valid
        if (sortFunctions.find(command) == sortFunctions.end() && command != "all")
        {
            cerr << "Error: Invalid command" << endl;
            return 1;
        }

        // read test cases from file
        vector<vector<int>> arraylist;
        if (!readFile(arraylist))
        {
            return 1;
        }

        // time the sorting process
        clock_t start, end;
        double duration = 0;

        // if command is "all", sort with all algorithms
        if (command == "all")
        {
            map<string, float> durations;
            for (auto &sortFunction : sortFunctions)
            {
                duration = 0;

                // copy the arraylist
                vector<vector<int>> arraylist_copy(arraylist);

                start = clock();

                // pass the copy of the array to the sort function
                for (auto &array : arraylist_copy)
                {
                    sortFunction.second(array);
                }

                end = clock();
                duration = (double)(end - start) / CLOCKS_PER_SEC * 1000 / arraylist.size();

                // calculate the average duration
                durations[sortFunction.first] = duration;

                // test the sorted array
                test(arraylist_copy, sortFunction.first);
            }

            // sort the durations
            vector<pair<string, float>> sortedDurations(durations.begin(), durations.end());
            sort(sortedDurations.begin(), sortedDurations.end(), [](const pair<string, float> &a, const pair<string, float> &b)
                 { return a.second < b.second; });

            cout << "Performance:" << endl;
            for (auto &duration : sortedDurations)
            {
                cout << duration.first << ": " << duration.second << " ms per test case" << endl;
            }
        }
        else
        {
            duration = 0;
            cout << "Sorting with " << command << " sort..." << endl;
            start = clock();

            for (auto &array : arraylist)
            {
                sortFunctions[command](array);
            }

            end = clock();
            duration = (double)(end - start) / CLOCKS_PER_SEC * 1000 / arraylist.size();
            cout << "Time: " << duration << " ms per test case" << endl;

            // test the sorted array
            test(arraylist, command);
        }
        return 0;
    }
    return 0;
}

void random_shuffle(vector<int>::iterator first, vector<int>::iterator last)
{
    // Fisher-Yates shuffle algorithm
    // shuffle the elements in the range [first, last)
    for (auto i = last - first - 1; i > 0; i--)
    {
        auto j = rand() % (i + 1);
        swap(first[i], first[j]);
    }
}

void generateTestCases(int total_cases, int case_size)
{
    // TODO: More variety of test cases

    vector<vector<int>> arraylist;

    // divide the test cases into 6 categories
    // 1. unique random
    // 2. sorted
    // 3. reverse sorted
    // 4. partially sorted
    // 5. few unique
    // 6. random

    // generate unique random test cases
    for (int i = 0; i < total_cases / 6; i++)
    {
        vector<int> array;
        for (int j = 0; j < case_size; j++)
        {
            array.push_back(j);
        }
        random_shuffle(array.begin(), array.end());
        arraylist.push_back(array);
    }

    // generate sorted test cases
    for (int i = 0; i < total_cases / 6; i++)
    {
        vector<int> array;
        for (int j = 0; j < case_size; j++)
        {
            array.push_back(j);
        }
        arraylist.push_back(array);
    }

    // generate reverse sorted test cases
    for (int i = 0; i < total_cases / 6; i++)
    {
        vector<int> array;
        for (int j = case_size - 1; j >= 0; j--)
        {
            array.push_back(j);
        }
        arraylist.push_back(array);
    }

    // generate partially sorted test cases
    for (int i = 0; i < total_cases / 6; i++)
    {
        vector<int> array;
        for (int j = 0; j < case_size; j++)
        {
            array.push_back(j);
        }
        random_shuffle(array.begin(), array.end());
        for (int j = 0; j < case_size / 2; j++)
        {
            swap(array[j], array[j + 1]);
        }
        arraylist.push_back(array);
    }

    // generate few unique test cases
    for (int i = 0; i < total_cases / 6; i++)
    {
        vector<int> array;
        for (int j = 0; j < case_size; j++)
        {
            array.push_back(j % 10);
        }
        random_shuffle(array.begin(), array.end());
        arraylist.push_back(array);
    }

    // generate random test cases
    for (int i = 0; i < total_cases / 6; i++)
    {
        vector<int> array;
        for (int j = 0; j < case_size; j++)
        {
            array.push_back(rand());
        }
        arraylist.push_back(array);
    }

    // write the test cases to file
    writeFile(arraylist);
}

void writeFile(vector<vector<int>> &arraylist)
{
    // TODO: Optimize the file writing process

    ofstream file(
        "input.txt",
        ios::out | ios::app);
    if (file.is_open())
    {
        for (int i = 0; i < arraylist.size(); i++)
        {
            for (int j = 0; j < arraylist[i].size(); j++)
            {
                file << arraylist[i][j] << " ";
            }
            file << endl;
        }
        file.close();
    }
}

bool readFile(vector<vector<int>> &arraylist)
{
    // TODO: Optimize the file reading process

    ifstream file(
        "input.txt",
        ios::in);

    if (!file)
    {
        cerr << "Error opening file" << endl;
        cerr << "Please run './sort gen' to generate test cases" << endl;
        return false;
    }

    file.seekg(0, ios::end);
    int length = file.tellg();
    file.seekg(0, ios::beg);

    char *buffer = new char[length];
    file.read(buffer, length);
    file.close();

    string str(buffer);
    delete[] buffer;

    size_t pos = 0;
    string token;
    while ((pos = str.find("\n")) != string::npos)
    {
        token = str.substr(0, pos);
        vector<int> array;
        size_t pos2 = 0;
        while ((pos2 = token.find(" ")) != string::npos)
        {
            array.push_back(stoi(token.substr(0, pos2)));
            token.erase(0, pos2 + 1);
        }
        // array.push_back(stoi(token));
        arraylist.push_back(array);
        str.erase(0, pos + 1);
    }

    return true;
}

void printArray(vector<int> array)
{
    for (int i = 0; i < array.size(); i++)
    {
        cout << array[i] << " ";
    }
    cout << endl;
}

void printTestCaseSize(vector<vector<int>> arraylist)
{
    int total_cases = arraylist.size();
    cout << "Total test cases: " << total_cases << endl;
    cout << endl;
    for (int i = 0; i < arraylist.size(); i++)
    {
        cout << "Test case " << i + 1 << " (" << arraylist[i].size() << ")" << endl;
    }
}

void test(vector<vector<int>> arraylist, string sort_name)
{
    int incorrect_cases = 0;
    int total_cases = arraylist.size();
    // int first_incorrect_case = -1;

    // ensure each test case is increasing
    for (int i = 0; i < arraylist.size(); i++)
    {
        for (int j = 0; j < arraylist[i].size() - 1; j++)
        {
            if (arraylist[i][j] > arraylist[i][j + 1])
            {
                incorrect_cases++;
                // first_incorrect_case = i;
                break;
            }
        }
    }

    if (incorrect_cases)
    {
        cout << sort_name << " Test Failed: ";
        cout << (total_cases - incorrect_cases) << "/" << total_cases << endl;
    }
}

void bubbleSort(vector<int> &array)
{
    // repeatedly swap adjacent elements if they are in the wrong order

    for (int i = 0; i < array.size(); i++)
    {
        for (int j = 0; j < array.size() - i - 1; j++)
        {
            if (array[j] > array[j + 1])
            {
                swap(array[j], array[j + 1]);
            }
        }
    }
}

void selectionSort(vector<int> &array)
{
    // select the minimum element from the unsorted part and place it at the beginning of the sorted part

    for (int i = 0; i < array.size(); i++)
    {
        int min_index = i;
        for (int j = i + 1; j < array.size(); j++)
        {
            if (array[j] < array[min_index])
            {
                min_index = j;
            }
        }
        if (min_index != i)
            swap(array[i], array[min_index]);
    }
}

void insertionSort(vector<int> &array)
{
    // insert an element from the unsorted part into its correct position in the sorted part

    for (int i = 1; i < array.size(); i++)
    {
        int key = array[i];
        int j = i - 1;
        while (j >= 0 && array[j] > key)
        {
            array[j + 1] = array[j];
            j--;
        }
        array[j + 1] = key;
    }
}

void mergeSort(vector<int> &array)
{
    // divide the array into two halves and recursively sort them
    // merge the two sorted halves

    if (array.size() <= 1)
    {
        return;
    }

    int mid = array.size() / 2;
    vector<int> left(array.begin(), array.begin() + mid);
    vector<int> right(array.begin() + mid, array.end());

    mergeSort(left);
    mergeSort(right);

    int i = 0, j = 0, k = 0;
    while (i < left.size() && j < right.size())
    {
        if (left[i] < right[j])
        {
            array[k] = left[i];
            i++;
        }
        else
        {
            array[k] = right[j];
            j++;
        }
        k++;
    }

    while (i < left.size())
    {
        array[k] = left[i];
        i++;
        k++;
    }

    while (j < right.size())
    {
        array[k] = right[j];
        j++;
        k++;
    }
}

void quickSort(vector<int> &array)
{
    // select a pivot element and partition the array into two halves
    // elements less than the pivot are placed to the left and elements greater than the pivot are placed to the right
    // recursively sort the left and right halves

    if (array.size() <= 1)
    {
        return;
    }

    int pivot = array[array.size() / 2];
    vector<int> left, right, equal;
    for (int i = 0; i < array.size(); i++)
    {
        if (array[i] < pivot)
        {
            left.push_back(array[i]);
        }
        else if (array[i] > pivot)
        {
            right.push_back(array[i]);
        }
        else
        {
            equal.push_back(array[i]);
        }
    }

    quickSort(left);
    quickSort(right);

    int k = 0;
    for (int i = 0; i < left.size(); i++)
    {
        array[k] = left[i];
        k++;
    }
    for (int i = 0; i < equal.size(); i++)
    {
        array[k] = equal[i];
        k++;
    }
    for (int i = 0; i < right.size(); i++)
    {
        array[k] = right[i];
        k++;
    }
}

void heapSort(vector<int> &array)
{
    // build a max heap from the array
    // repeatedly extract the maximum element from the heap and place it at the end of the array

    int n = array.size();

    for (int i = n / 2 - 1; i >= 0; i--)
    {
        heapify(array, n, i);
    }

    for (int i = n - 1; i > 0; i--)
    {
        swap(array[0], array[i]);
        heapify(array, i, 0);
    }
}

void countingSort(vector<int> &array)
{
    // count the number of occurrences of each element and store it in a count array
    // calculate the prefix sum of the count array
    // place the elements in the output array based on the prefix sum

    int max = *max_element(array.begin(), array.end());
    int min = *min_element(array.begin(), array.end());
    int range = max - min + 1;

    vector<int> count(range, 0);
    vector<int> output(array.size(), 0);

    for (int i = 0; i < array.size(); i++)
    {
        count[array[i] - min]++;
    }

    for (int i = 1; i < count.size(); i++)
    {
        count[i] += count[i - 1];
    }

    for (int i = array.size() - 1; i >= 0; i--)
    {
        output[count[array[i] - min] - 1] = array[i];
        count[array[i] - min]--;
    }

    for (int i = 0; i < array.size(); i++)
    {
        array[i] = output[i];
    }
}

void radixSort(vector<int> &array)
{
    // sort the elements based on the digits in each place value
    // sort the elements based on the least significant digit to the most significant digit

    int max = *max_element(array.begin(), array.end());
    int exp = 1;
    vector<int> output(array.size(), 0);

    while (max / exp > 0)
    {
        vector<int> count(10, 0);

        for (int i = 0; i < array.size(); i++)
        {
            count[(array[i] / exp) % 10]++;
        }

        for (int i = 1; i < count.size(); i++)
        {
            count[i] += count[i - 1];
        }

        for (int i = array.size() - 1; i >= 0; i--)
        {
            output[count[(array[i] / exp) % 10] - 1] = array[i];
            count[(array[i] / exp) % 10]--;
        }

        for (int i = 0; i < array.size(); i++)
        {
            array[i] = output[i];
        }

        exp *= 10;
    }
}

void bucketSort(vector<int> &array)
{
    // divide the array into smaller subarrays and sort them
    // concatenate the sorted subarrays

    int n = array.size();
    if (n <= 1)
        return;

    int max = *max_element(array.begin(), array.end());
    int min = *min_element(array.begin(), array.end());
    int range = max - min;

    // Handle edge case where all elements are the same
    if (range == 0)
        return;

    // Dynamic bucket size
    int bucket_size = n;
    vector<vector<int>> buckets(bucket_size);

    // Place elements into buckets
    for (int i = 0; i < n; i++)
    {
        int index = (array[i] - min) * bucket_size / (range + 1);
        buckets[index].push_back(array[i]);
    }

    // Sort individual buckets and concatenate
    int k = 0;
    for (int i = 0; i < bucket_size; i++)
    {
        sort(buckets[i].begin(), buckets[i].end());
        for (int j = 0; j < buckets[i].size(); j++)
        {
            array[k++] = buckets[i][j];
        }
    }
}

void shellSort(vector<int> &array)
{
    // improve insertion sort by comparing elements that are far apart
    // and reducing the gap between elements

    int n = array.size();
    for (int gap = n / 2; gap > 0; gap /= 2)
    {
        for (int i = gap; i < n; i++)
        {
            int temp = array[i];
            int j;
            for (j = i; j >= gap && array[j - gap] > temp; j -= gap)
            {
                array[j] = array[j - gap];
            }
            array[j] = temp;
        }
    }
}

void cocktailSort(vector<int> &array)
{
    // improve bubble sort by moving the largest element to the rightmost position
    // and the smallest element to the leftmost position

    int n = array.size();
    bool swapped = true;
    int start = 0;
    int end = n - 1;

    while (swapped)
    {
        swapped = false;

        for (int i = start; i < end; i++)
        {
            if (array[i] > array[i + 1])
            {
                swap(array[i], array[i + 1]);
                swapped = true;
            }
        }

        if (!swapped)
        {
            break;
        }

        swapped = false;
        end--;

        for (int i = end - 1; i >= start; i--)
        {
            if (array[i] > array[i + 1])
            {
                swap(array[i], array[i + 1]);
                swapped = true;
            }
        }

        start++;
    }
}

void combSort(vector<int> &array)
{
    // improve bubble sort by reducing the gap between elements

    int n = array.size();
    int gap = n;
    bool swapped = true;

    while (gap > 1 || swapped)
    {
        if (gap > 1)
        {
            gap = gap / 1.3;
        }

        swapped = false;

        for (int i = 0; i < n - gap; i++)
        {
            if (array[i] > array[i + gap])
            {
                swap(array[i], array[i + gap]);
                swapped = true;
            }
        }
    }
}

void gnomeSort(vector<int> &array)
{
    // improve insertion sort by moving the element to its correct position
    // by comparing it with the previous element

    int n = array.size();
    int i = 0;

    while (i < n)
    {
        if (i == 0 || array[i] >= array[i - 1])
        {
            i++;
        }
        else
        {
            swap(array[i], array[i - 1]);
            i--;
        }
    }
}
