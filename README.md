# C++ Sentiment Analysis via Linked Lists

A custom-built C++ application that performs sentiment analysis on TripAdvisor hotel reviews. Instead of relying on standard library containers like `std::vector` or `std::list`, this project utilizes from-scratch Singly Linked Lists to manage memory, store dictionary data, and process text algorithms, demonstrating a strong understanding of core computer science fundamentals.

## 🚀 Core Features

* **Custom Data Structures:** Implements two distinct linked list classes (`LinkedList` for word dictionaries and `LinkedListReviews` for tuple-like review data).
* **Lexical Text Parsing:** Ingests raw text from CSV files, tokenizes sentences, and normalizes capitalization using `<algorithm>` and `<sstream>` to ensure accurate word matching.
* **Algorithmic Sentiment Scoring:** Calculates a raw sentiment delta (Positive Count - Negative Count), normalizes it against a clamped scale (0.0 to 1.0), and translates it into a standard 1-to-5 star rating.
* **Automated File I/O:** Dynamically reads from `positive-words.txt`, `negative-words.txt`, and `tripadvisor_hotel_reviews.csv`, and outputs the analytical breakdown to a new `sentiment_analysis_results.csv` file.

## 🛠️ Data Structures & Tech Stack

* **Language:** C++
* **Custom Data Structures:**
  * `Node`: Custom struct utilizing raw pointers for dynamic memory allocation.
  * `LinkedList`: Manages singular string nodes for fast sequential dictionary lookups.
  * `LinkedListReviews`: Manages complex nodes containing both string data (review text) and integer data (original rating).
* **Libraries Used:** `<iostream>`, `<fstream>`, `<sstream>`, `<string>`, `<limits>`, `<algorithm>`

## ⚙️ How to Run Locally

1. Ensure you have a C++ compiler installed (e.g., GCC, MSVC, Clang).
2. Clone this repository to your local machine.
3. Ensure the following three files exist in the same root directory as your executable:
   * `positive-words.txt` (A list of positive dictionary words, one per line)
   * `negative-words.txt` (A list of negative dictionary words, one per line)
   * `tripadvisor_hotel_reviews.csv` (The target dataset)
4. Compile the source code:
   ```bash
   g++ -o sentiment_analyzer main.cpp
