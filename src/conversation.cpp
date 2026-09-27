#include "core/conversation.h"
#include <stdexcept>

// Constructor
Conversation::Conversation()
{
}

// Destructor
Conversation::~Conversation()
{
    delete[] data_;
}

// Copy constructor
Conversation::Conversation(const Conversation& other)
{
    size_ = other.size_;
    capacity_ = other.capacity_;

    if (capacity_ > 0) {
        data_ = new Message[capacity_];

        for (std::size_t i = 0; i < size_; ++i) {
            data_[i] = other.data_[i];
        }
    }
}

// Copy assignment operator
Conversation& Conversation::operator=(const Conversation& other)
{
    if (this != &other) {

        Message* new_data = nullptr;

        if (other.capacity_ > 0) {
            new_data = new Message[other.capacity_];

            for (std::size_t i = 0; i < other.size_; ++i) {
                new_data[i] = other.data_[i];
            }
        }

        delete[] data_;

        data_ = new_data;
        size_ = other.size_;
        capacity_ = other.capacity_;
    }

    return *this;
}

// Move constructor
Conversation::Conversation(Conversation&& other) noexcept
{
    data_ = other.data_;
    size_ = other.size_;
    capacity_ = other.capacity_;

    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
}

// Move assignment operator
Conversation& Conversation::operator=(Conversation&& other) noexcept
{
    if (this != &other) {

        delete[] data_;

        data_ = other.data_;
        size_ = other.size_;
        capacity_ = other.capacity_;

        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    return *this;
}

// Add a Message to the end of the Conversation
void Conversation::append(Message m)
{
    if (size_ == capacity_) {

        std::size_t new_capacity;

        if (capacity_ == 0) {
            new_capacity = 1;
        }
        else {
            new_capacity = capacity_ * 2;
        }

        Message* new_data = new Message[new_capacity];

        for (std::size_t i = 0; i < size_; ++i) {
            new_data[i] = data_[i];
        }

        delete[] data_;

        data_ = new_data;
        capacity_ = new_capacity;
    }

    data_[size_] = m;
    ++size_;
}

// Return number of Messages
std::size_t Conversation::size() const noexcept
{
    return size_;
}

// Return Message at position i
const Message& Conversation::at(std::size_t i) const
{
    if (i >= size_) {
        throw std::out_of_range("Conversation index out of range");
    }

    return data_[i];
}

// Pointer to first Message
const Message* Conversation::begin() const noexcept
{
    return data_;
}

// Pointer one past the last Message
const Message* Conversation::end() const noexcept
{
    return data_ + size_;
}