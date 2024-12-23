# 排序算法的時間複雜度排序與分析

## 1. 時間複雜度排序

### 最差情況時間複雜度

按照時間複雜度由高到低排序：

1. **\( O(n^2) \)**: Bubble, Selection, Insertion, Gnome, Cocktail, Comb (若未優化)
2. **\( O(n \log n) \)**: Merge, Quick, Heap, Shell (特定增量序列)
3. **\( O(n + k) \)**: Counting, Radix, Bucket  
   （當數據範圍 \(k\) 小於 \(n^2\)，這些算法通常是線性時間）

---

### 平均情況時間複雜度

按照時間複雜度由高到低排序：

1. **\( O(n^2) \)**: Bubble, Selection, Insertion, Gnome, Cocktail, Comb (若未優化)
2. **\( O(n \log n) \)**: Merge, Quick, Heap, Shell (特定增量序列)
3. **\( O(n + k) \)**: Counting, Radix, Bucket

---

### 最佳情況時間複雜度

按照時間複雜度由高到低排序：

1. **\( O(n^2) \)**: Selection, Gnome
2. **\( O(n) \)**: Bubble, Insertion, Cocktail, Comb (數據已近乎排序)
3. **\( O(n \log n) \)**: Merge, Quick, Heap, Shell
4. **\( O(n + k) \)**: Counting, Radix, Bucket

---

## 2. 時間複雜度與原因

### \( O(n^2) \)

這些算法通常使用嵌套迴圈進行比較和交換：

- **Bubble Sort**：每次遍歷將最大的元素冒泡到正確位置。
- **Selection Sort**：每次選擇最小或最大元素放到正確位置。
- **Insertion Sort**：將每個元素插入到已排序子陣列中。
- **Gnome Sort**：類似插入排序，逐步修正順序。
- **Cocktail Sort**：雙向冒泡排序。
- **Comb Sort**：冒泡排序的改進版本，使用逐漸縮小的間隔比較。

**原因：** 它們的核心操作是基於逐一比較，且需要多次迭代，無法有效分治。

---

### \( O(n \log n) \)

這些算法使用「分治策略」或「堆操作」提高效率：

- **Merge Sort**：分治法，將數組分為小段並合併排序。
- **Quick Sort**：選擇樞軸，將數據分區，再遞歸排序分區。
- **Heap Sort**：基於二叉堆結構，利用最大堆或最小堆進行排序。
- **Shell Sort**：改進版插入排序，通過逐步減小間隔進行排序。

**原因：** 這些算法通過分治或跳過大量比較操作來減少比較次數。

---

### \( O(n + k) \)

這些算法適用於特定條件下的數據（如範圍有限或整數鍵）：

- **Counting Sort**：對數據計數，然後根據計數排序。
- **Radix Sort**：按位或數字基數進行排序，依賴穩定的輔助排序。
- **Bucket Sort**：將數據分入不同桶，對每個桶進行單獨排序。

**原因：** 它們依賴數據特性來避免比較操作，通常在數據分布均勻或範圍有限時效率較高。

---

## 3. 排序算法的比較總結

| 排序算法  | 最差情況       | 平均情況       | 最佳情況       | 穩定性 | 適用場景                             |
| --------- | -------------- | -------------- | -------------- | ------ | ------------------------------------ |
| Bubble    | \(O(n^2)\)     | \(O(n^2)\)     | \(O(n)\)       | 穩定   | 小數據集，幾乎有序的數據             |
| Selection | \(O(n^2)\)     | \(O(n^2)\)     | \(O(n^2)\)     | 不穩定 | 小數據集                             |
| Insertion | \(O(n^2)\)     | \(O(n^2)\)     | \(O(n)\)       | 穩定   | 小數據集，幾乎有序的數據             |
| Merge     | \(O(n\log n)\) | \(O(n\log n)\) | \(O(n\log n)\) | 穩定   | 通用排序，高效率需求                 |
| Quick     | \(O(n^2)\)     | \(O(n\log n)\) | \(O(n\log n)\) | 不穩定 | 通用排序，高效率需求                 |
| Heap      | \(O(n\log n)\) | \(O(n\log n)\) | \(O(n\log n)\) | 不穩定 | 通用排序，Memory-sensitive scenarios |
| Counting  | \(O(n + k)\)   | \(O(n + k)\)   | \(O(n + k)\)   | 穩定   | 整數鍵，範圍有限                     |
| Radix     | \(O(n + k)\)   | \(O(n + k)\)   | \(O(n + k)\)   | 穩定   | 整數鍵或特定類型鍵                   |
| Bucket    | \(O(n^2)\)     | \(O(n + k)\)   | \(O(n + k)\)   | 穩定   | 實數範圍，數據分布均勻               |
| Shell     | \(O(n^2)\)     | \(O(n\log n)\) | \(O(n\log n)\) | 不穩定 | 通用排序                             |
| Cocktail  | \(O(n^2)\)     | \(O(n^2)\)     | \(O(n)\)       | 穩定   | 小數據集，幾乎有序的數據             |
| Comb      | \(O(n^2)\)     | \(O(n^2)\)     | \(O(n)\)       | 不穩定 | 改進版冒泡排序，低優先場景           |
| Gnome     | \(O(n^2)\)     | \(O(n^2)\)     | \(O(n)\)       | 穩定   | 小數據集，簡化實現需求               |

---

## 4. 選擇排序算法的建議

- 使用 \(O(n \log n)\) 算法（如 Quick、Merge、Heap）處理大數據集。
- 當數據量小或幾乎有序時，使用 Insertion Sort 或 Bubble Sort。
- 當範圍有限或數據特性明確時，使用 Counting Sort 或 Radix Sort。
