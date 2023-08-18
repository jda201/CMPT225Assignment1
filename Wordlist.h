// Wordlist.h

#pragma once

#include "Wordlist_base.h"
#include <cassert>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

class Wordlist : public Wordlist_base
{
    struct Node
    {
        string word;
        int count;
        Node *next;
        Node(const string& word, Node* next = nullptr): word(word), next(next) {}
    };

     Node *head = nullptr;
     int word_count = 0;

public:

    // Default Constructor
    Wordlist() {}

    bool empty() const
    {
        return head == nullptr;
    }

    // Constructor that takes the name of a file as input and adds all the words in that file to Wordlist 
    Wordlist(const string& filename)  
    {
        ifstream file(filename);
        if (!file) {
            cerr << "Unable to open file " << filename << endl;
            exit(1); 
        }
        string word;
        while (file >> word) 
        {
            add_word(word);
        }
    }

    // Returns the number of times w occurs as a word in the list
    int get_count(const string &w) const
    {
        int count = 0;
        for (Node* p = head; p!= nullptr; p = p->next) 
        {
            if (p->word == w) 
            {
                count++;
            }
        }
        return count;
    }

    // Returns the number of different words in the list
    int num_different_words() const 
    {
        int count = 0;
        for (Node* p = head; p != nullptr; p = p->next) 
        {
            Node* node;
            for (node = p->next; node != nullptr; node = node->next) 
            {
                // If the word is a duplicate, don't increment count
                if (node->word == p->word) 
                {
                    break;
                }
            }
            // If no duplicate word found, increment count
            if (node == nullptr) 
            {
                count++;
            }
        }
        return count;
    }

    // Returns the total number of words in the list
    int total_words() const
    {
        return word_count;
    }

    // Returns true if the words in the list are in ascending sorted order, false otherwise
    bool is_sorted() const
    {
        // If the list is empty or contains only one word, then the list is sorted
        if (head == nullptr || head->next == nullptr) 
        {
            return true;
        }

        Node* p = head;
        while (p->next != nullptr)
        {
            // If the current word in the list is lexicographically greater than the next word,
            // then the list is not in ascending sorted order
            if (p->word > p->next->word)
            {
                return false;
            }
            p = p->next;
        }

        return true; 
    }

    // Returns the most frequent word in the list and its count
    string most_frequent() const
    {
        int max_count = 0;
        Node *p = head;
        string mostFrequent;
        while (p != nullptr)
        {
            string current_word = p->word;
            int current_count = 1;
            for (Node* node = p->next; node != nullptr; node = node->next)
            {
                if (node->word == current_word) 
                {
                    current_count++;
                }
            }
            // Each time a word is found with a greater count than the previous word, 
            // update the most frequent word and its count
            if (current_count > max_count) 
            {
                max_count = current_count;
                mostFrequent = current_word;
            }
            p = p->next;
        }
        string total = mostFrequent + " " + to_string(max_count);
        return total;
    }

    // Returns the number of words in the list that only appear once
    int num_singletons() const
    {
        int count = 0;
        for (Node* p = head; p != nullptr; p = p->next) 
        {
            // If the word only appears once in the list then increase count
            if (get_count(p->word) == 1) 
            {
                count++;
            }
        }
        return count;
    }

    // Adds w to the word list. If w is already in the list, then increment its
    // count, otherwise add a new Node (with count 1) at the alphabetically
    // correct location
    void add_word(const string &w) 
    {
        Node* node = new Node{w, nullptr};
        // If the list is empty or w is lexicographically smaller than the first word 
        // then add w to the front of the list and set w as the new first word
        if (head == nullptr || w <= head->word)
        {
            node->next = head;
            head = node;
        }   
        // Traverse the list until a word is found where its next word is lexicographically
        // greater than w, and insert w between the word and its next word
        else 
        {
            Node* p = head;
            while (p->next != nullptr && p->next->word < w) 
            {
                p = p->next;
            }
            node->next = p->next;
            p->next = node;
        }
        word_count++;
        assert(is_sorted());
    }
    
    // Print the words in the list in ascending sorted order with their counts
    void print_words() const
     {
        Node* p = head;
        int count = 1;
        while (p != nullptr) 
        {
            bool duplicate = false;
            Node* node = p->next;
            while (node != nullptr)
            {
                // If the word is a duplicate then don't print again
                if (node->word == p->word)
                {
                    duplicate = true;
                    break;
                }
                node = node->next;
            }
            // If the word is not a duplicate then print the word along with its count
            if (!duplicate)
            {
                cout << count << ". {\"" << p->word << "\", " << get_count(p->word) << "}";
                cout << endl;
                count++;
            }
            p = p->next;
        }
    }

    string pop_front()
    {
        // Causes a crash if !empty() is false
        assert(!empty());
        string s = head->word;
        Node *p = head;
        head = head->next;
        delete p;
        return s;
    }

    // Remove all nodes from the list
    void clear()
    {
        while (!empty())
        {
            pop_front();
        }
    }

    // Destructor
    ~Wordlist()
    {
        clear();
        cout << "Wordlist destructor called" << endl;
    }

};
