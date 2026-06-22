#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm> // For std::transform

// Node structure for linked list
struct Node {
    std::string data;
    Node* next;
};

// Linked list class for words (positive/negative)
class LinkedList {
private:
    Node* head;
public:
    LinkedList() { head = nullptr; }

    void add(std::string data) {
        Node* newNode = new Node();
        newNode->data = data;
        newNode->next = head;
        head = newNode;
    }

    bool contains(const std::string& word) {
        Node* temp = head;
        while (temp != nullptr) {
            if (temp->data == word) return true;
            temp = temp->next;
        }
        return false;
    }

    ~LinkedList() {
        Node* current = head;
        while (current != nullptr) {
            Node* nextNode = current->next;
            delete current;
            current = nextNode;
        }
    }
};

// Linked list class for reviews
class LinkedListReviews {
private:
    Node* head;
public:
    LinkedListReviews() { head = nullptr; }

    void addReview(const std::string& text, int rating) {
        std::string combined = text + "," + std::to_string(rating);
        Node* newNode = new Node();
        newNode->data = combined;
        newNode->next = head;
        head = newNode;
    }

    Node* getHead() {
        return head;
    }

    ~LinkedListReviews() {
        Node* current = head;
        while (current != nullptr) {
            Node* nextNode = current->next;
            delete current;
            current = nextNode;
        }
    }
};

void loadWords(const std::string& filename, LinkedList& list) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }

    std::string word;
    while (file >> word) {
        list.add(word);
    }

    file.close();
}

void loadReviews(const std::string& filename, LinkedListReviews& list) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::string reviewText;
        int rating = 0;

        size_t commaPos = line.find_last_of(',');
        if (commaPos != std::string::npos) {
            reviewText = line.substr(0, commaPos);
            try {
                rating = std::stoi(line.substr(commaPos + 1));
            }
            catch (const std::invalid_argument& e) {
                std::cerr << "Invalid rating format in line: " << line << std::endl;
                continue;
            }
        }
        else {
            reviewText = line;
        }

        list.addReview(reviewText, rating);
    }

    file.close();
}

int countPositiveWords(const std::string& review, LinkedList& positiveWords) {
    int count = 0;
    std::string word;
    std::stringstream ss(review);
    while (ss >> word) {
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);
        if (positiveWords.contains(word)) {
            count++;
        }
    }
    return count;
}

int countNegativeWords(const std::string& review, LinkedList& negativeWords) {
    int count = 0;
    std::string word;
    std::stringstream ss(review);
    while (ss >> word) {
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);
        if (negativeWords.contains(word)) {
            count++;
        }
    }
    return count;
}

float customClamp(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

int calculateSentimentScore(int posCount, int negCount) {
    int rawScore = posCount - negCount;
    float normalizedScore = static_cast<float>(rawScore + 10) / 20;
    normalizedScore = customClamp(normalizedScore, 0.0f, 1.0f);
    return static_cast<int>(1 + (4 * normalizedScore));
}

void saveToCSV(const std::string& filename, const std::string& review, int posCount, int negCount, int sentimentScore) {
    std::ofstream outFile(filename, std::ios::app);
    if (!outFile.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }
    outFile << review << "," << posCount << "," << negCount << "," << sentimentScore << "\n";
    outFile.close();
}

int main() {
    LinkedList positiveWords, negativeWords;
    LinkedListReviews reviews;

    loadWords("positive-words.txt", positiveWords);
    loadWords("negative-words.txt", negativeWords);
    loadReviews("tripadvisor_hotel_reviews.csv", reviews);

    std::ofstream outFile("sentiment_analysis_results.csv");
    outFile << "Review,Positive Word Count,Negative Word Count,Sentiment Score\n";
    outFile.close();

    std::cout << "Processing reviews...\n";

    Node* reviewNode = reviews.getHead();
    int reviewNumber = 1;
    while (reviewNode != nullptr) {
        std::string currentLine = reviewNode->data;
        std::string currentReview;
        int currentRating;

        size_t commaPos = currentLine.find_last_of(',');
        if (commaPos != std::string::npos) {
            currentReview = currentLine.substr(0, commaPos);
            currentRating = std::stoi(currentLine.substr(commaPos + 1));
        }

        int posCount = countPositiveWords(currentReview, positiveWords);
        int negCount = countNegativeWords(currentReview, negativeWords);
        int sentimentScore = calculateSentimentScore(posCount, negCount);

        saveToCSV("sentiment_analysis_results.csv", currentReview, posCount, negCount, sentimentScore);

        reviewNode = reviewNode->next;
        reviewNumber++;
    }

    std::cout << "Sentiment analysis completed. Results saved in 'sentiment_analysis_results.csv'.\n";
    return 0;
}
