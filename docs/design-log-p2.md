# Design Log — Project 2
## Growth factor and amortized cost
Within my Conversation class work, a growth factor of 2 was chosen by me for expanding storage. Experts claim that doubling storage sizes helps prevent constant memory allocations. Once an array reaches full capacity, a new array possessing two times the capacity of the earlier array is produced. Capacity starting from zero remains a single special condition. Under that condition, the initial allocation receives a capacity of 1. Consequently, progression of the capacity continues upward through 0, 1, 2, 4, 8, 16, and onward in sequential doubling.

Whenever array expansion becomes mandatory, prior Message objects are transferred into the new array through copying. Cost for an individual append operation rises higher during the instance where memory reallocation happens. Yet, memory reallocation is not executed during every append cycle. More messages can safely enter the structure before another reallocation arrives while the array becomes bigger. Due to capacity multiplying twofold every time, the cumulative copying across repeated append actions matches the total count of inserted messages. It has been observed that append maintains an amortized time complexity of O(1), even though distinct append events consume O(n) duration during the moments the array must enlarge.

Verification of such actions was performed through appending message items and inspecting the sequential capacity shifts across 0, 1, 2, 4, and 8. Many people believe that thorough testing is required to confirm structural stability, which is why size numbers and preserved messages were verified to ensure accuracy persisted following reallocation.


## Rule of Five evidence
 Since my Conversation class uses an allocated array to store the messages I needed to use the Rule of Five to make sure the memory was handled correctly. I implemented a destructor, copy constructor, copy assignment operator, move constructor, and move assignment operator. Firstly, the destructor deletes the array when a Conversation object is no longer being used. For the copy constructor, I created a new array and copied the messages from the original Conversation into it. This is important because I did not want two Conversation objects pointing to the same array. I handled the copy assignment operator in a similar way by creating a new copy of the data and getting rid of the old data.

The move constructor and move assignment operator work differently. Instead of making another copy of all the messages, I transfer the existing data pointer, size, and capacity to the new Conversation. After the move, I set the old Conversation's pointer to nullptr and its size and capacity to zero. This leaves the old object empty and prevents both objects from trying to delete the same memory.

I also wrote tests to make sure this worked correctly. For a copy, I checked that both Conversations had the same message but used different memory addresses. I then added another message to the original and made sure the copy did not change. For a move, I checked that the new Conversation received the original data pointer and that the old Conversation was empty afterward.


## Sentinel scanner: bounded pending_ proof
I used pending_ to hold the characters at the end of a chunk that could possibly be part of the sentinel. The important part is that I only needed to hold at most one less character than the length of the sentinel. Anything before that is safe to return because it can no longer be the beginning of a complete sentinel. This keeps pending_ from continuing to grow as more chunks are received.


## What I would change differently
One thing I would totaly do differently and will definitely make sure to change in the next project is when I create the tests. Next time I would like to not wait to I am done implementing to start testing. It makes it a lot more confusing nd complicated if you wait till the end like I did.