// tests/p2/test_p2.cpp
//
// YOUR test suite goes here. At least 12 assert-based test cases — see
// spec §5 for the required categories and the sample test for the
// expected level of rigor.
//
// This file is a stub so the project builds out of the box; replace the
// body of main() with your own tests.

#include "core/conversation.h"
#include "core/message.h"
#include "core/sentinel_scanner.h"
#include "harness/harness.h"
#include "model/replay_client.h"
#include "model/scripted_client.h"
#include <stdexcept>

#include <cassert>

int main() {
    // my tests.

    //firsttest
        Message m;
    assert(m.role() == Role::System);
    assert(m.content() == "");


    // second test
    
    Message m2(Role::User, "Hello");

    assert(m2.role() == Role::User);
    assert(m2.content() == "Hello");


    //third test (empty)
    Conversation c;

    assert(c.size() == 0); //no text added
    assert(c.begin() == c.end());




    //Fourth test (appending)
    Conversation c2;

    c2.append(Message(Role::User, "Hello"));

    assert(c2.size() == 1);
    assert(c2.at(0).role() == Role::User);
    assert(c2.at(0).content() == "Hello");

        //Fifth test (more than one messages)
    Conversation c3;

    c3.append(Message(Role::System, "System message"));
    c3.append(Message(Role::User, "First user message"));
    c3.append(Message(Role::Assistant, "First assistant message"));
    c3.append(Message(Role::User, "Second user message"));

    assert(c3.size() == 4);

    assert(c3.at(0).role() == Role::System);
    assert(c3.at(1).role() == Role::User);
    assert(c3.at(2).role() == Role::Assistant);
    assert(c3.at(3).role() == Role::User);

    assert(c3.at(3).content() == "Second user message");



    //Sixth test exception case if message does not exist
    Conversation c4;

    bool threw = false;

    try {
        c4.at(0);
    }
    catch (const std::out_of_range&) {
        threw = true;
    }

    assert(threw);



    return 0;

    

}
