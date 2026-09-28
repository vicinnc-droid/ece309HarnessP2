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
#include <memory>
#include <string>
#include <cassert>

class TestInput : public InputSource {
public:
    TestInput() : count_(0) {}

    std::string read_line() override {
        ++count_;
        return "hello";
    }

    bool is_eof() const override {
        return false;
    }

private:
    int count_;
};

class TestOutput : public OutputSink {
public:
    void write(std::string_view text) override {
        output_ += text;
    }

    std::string output_;
};

int main() {
    // my tests.

    //Empty Conversation Bounds
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




    //appending
    Conversation c2;

    c2.append(Message(Role::User, "Hello"));

    assert(c2.size() == 1);
    assert(c2.at(0).role() == Role::User);
    assert(c2.at(0).content() == "Hello");

    //more than one messages
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


    //Rule of Five (Copy)
    Conversation original;
    original.append(Message(Role::User, "Original message"));
    

    Conversation copy(original);

    assert(copy.size() == 1);
    assert(copy.data() != original.data());
    assert(copy.at(0).role() == Role::User);
    assert(copy.at(0).content() == "Original message");
    //copying the conversation
    original.append(Message(Role::Assistant, "New message"));

    assert(original.size() == 2);
    assert(copy.size() == 1);
    assert(copy.at(0).content() == "Original message");




    //Rule of Five (Move)
    //moving the constructor
    Conversation move_original;
    move_original.append(Message(Role::User, "Move me"));

    const Message* original_data = move_original.data();

    Conversation moved(std::move(move_original));

    assert(moved.size() == 1);
    assert(moved.at(0).content() == "Move me");

    // Make sure the move stole the original pointer
    assert(moved.data() == original_data);

    // Make sure the source was zeroed out
    assert(move_original.data() == nullptr);
    assert(move_original.size() == 0);
    assert(move_original.capacity() == 0);




    //(chunk contains sentinel)
    SentinelScanner scanner("<|end_conversation|>");

    SentinelScanner::Out result =
        scanner.feed("Hello<|end_conversation|>");

    assert(result.safe_text == "Hello");
    assert(result.sentinel_found == true);


    //test (two chunks contain sentinel)
    SentinelScanner scanner2("<|end_conversation|>");

    SentinelScanner::Out part1 =
        scanner2.feed("Goodbye<|end_");
    SentinelScanner::Out part2 =
        scanner2.feed("conversation|>");
    assert(part1.sentinel_found == false);
    assert(part2.sentinel_found == true);

    assert(part1.safe_text + part2.safe_text == "Goodbye");

    //test flush
    SentinelScanner scanner3("<|end_conversation|>");

    SentinelScanner::Out before_flush =
        scanner3.feed("Short text");

    SentinelScanner::Out flushed =
        scanner3.flush();

    assert(before_flush.sentinel_found == false);
    assert(flushed.sentinel_found == false);
    assert(before_flush.safe_text + flushed.safe_text == "Short text");

    

    //Testing copying
    Conversation assign_original;
    assign_original.append(Message(Role::User, "Copy assignment"));

    Conversation assigned;
    assigned = assign_original;

    assert(assigned.size() == 1);
    assert(assigned.at(0).content() == "Copy assignment");

    assign_original.append(Message(Role::Assistant, "Another message"));

    assert(assign_original.size() == 2);
    assert(assigned.size() == 1);




    //Testing moving assignments
        Conversation move_assign_original;
    move_assign_original.append(Message(Role::Assistant, "Move assignment"));

    Conversation move_assigned;
    move_assigned = std::move(move_assign_original);

    assert(move_assigned.size() == 1);
    assert(move_assigned.at(0).content() == "Move assignment");
    assert(move_assign_original.size() == 0);



    // Scanner (Clean Text)
    // Clean text
    SentinelScanner clean_scanner("<|end_conversation|>");

    SentinelScanner::Out clean1 =
        clean_scanner.feed("Hello, this is normal text.");

    SentinelScanner::Out clean2 =
        clean_scanner.flush();

    assert(clean1.sentinel_found == false);
    assert(clean2.sentinel_found == false);
    assert(clean1.safe_text + clean2.safe_text ==
           "Hello, this is normal text.");




    //Scanner (Split Sentinel)
    //tetsing sentinel at different boundaries
    std::string sentinel = "<|end_conversation|>";

    for (std::size_t i = 1; i < sentinel.size(); ++i) {
        SentinelScanner split_scanner(sentinel);

        std::string first_chunk = "Hello" + sentinel.substr(0, i);
        std::string second_chunk = sentinel.substr(i);

        SentinelScanner::Out first =
            split_scanner.feed(first_chunk);

        SentinelScanner::Out second =
            split_scanner.feed(second_chunk);

        assert(first.sentinel_found == false);
        assert(second.sentinel_found == true);
        assert(first.safe_text + second.safe_text == "Hello");
    }





    // Scanner (False Alarms)
    // Making sure something similar to the sentinel does not stop conversation
    SentinelScanner false_scanner("<|end_conversation|>");

    SentinelScanner::Out false1 =
        false_scanner.feed("Hello <|end_world|> goodbye");

    SentinelScanner::Out false2 =
        false_scanner.flush();

    assert(false1.sentinel_found == false);
    assert(false2.sentinel_found == false);
    assert(false1.safe_text + false2.safe_text ==
           "Hello <|end_world|> goodbye");




    // Scanner (Bounded Memory)
    // scanner bounded memory test
    std::string bounded_sentinel = "<|end_conversation|>";
    SentinelScanner bounded_scanner(bounded_sentinel);

    for (int i = 0; i < 1000; ++i) {
        bounded_scanner.feed("<|end_conversatio");

        assert(bounded_scanner.pending_size()
               <= bounded_sentinel.size() - 1);
    }




    //Growth behavior
    // conversation growth behavior test
    Conversation growth;

    assert(growth.size() == 0);
    assert(growth.capacity() == 0);

    growth.append(Message(Role::User, "1"));
    assert(growth.size() == 1);
    assert(growth.capacity() == 1);

    growth.append(Message(Role::User, "2"));
    assert(growth.size() == 2);
    assert(growth.capacity() == 2);

    growth.append(Message(Role::User, "3"));
    assert(growth.size() == 3);
    assert(growth.capacity() == 4);

    growth.append(Message(Role::User, "4"));
    assert(growth.size() == 4);
    assert(growth.capacity() == 4);

    growth.append(Message(Role::User, "5"));
    assert(growth.size() == 5);
    assert(growth.capacity() == 8);

    assert(growth.at(0).content() == "1");
    assert(growth.at(4).content() == "5");




    //System Message Ordering
    //testing that system message remains first
    Conversation system_order;

    system_order.append(Message(Role::System, "System"));
    system_order.append(Message(Role::User, "Hello"));
    system_order.append(Message(Role::Assistant, "Hi"));

    assert(system_order.size() == 3);
    assert(system_order.at(0).role() == Role::System);
    assert(system_order.at(0).content() == "System");
    assert(system_order.at(1).role() == Role::User);
    assert(system_order.at(2).role() == Role::Assistant);





    
    //Harness Turn Limit Test
{
    auto model = std::make_unique<ScriptedModelClient>(
        "tests/p2/turn_limit.script"
    );

    HarnessConfig cfg;
    cfg.max_turns = 3;

    Harness harness(std::move(model), cfg);

    TestInput input;
    TestOutput output;

    StopReason reason = harness.run(input, output);

    assert(reason.kind == StopReason::Kind::TurnLimit);
    assert(harness.conversation().size() == 6);
}




    // Harness (Sentinel Halt)
{
    auto model = std::make_unique<ScriptedModelClient>(
        "tests/p2/sentinel_halt.script"
    );

    HarnessConfig cfg;
    cfg.max_turns = 5;

    Harness harness(std::move(model), cfg);

    TestInput input;
    TestOutput output;

    StopReason reason = harness.run(input, output);

    assert(reason.kind == StopReason::Kind::Sentinel);

    // It should stop after one user/assistant turn
    assert(harness.conversation().size() == 2);

    assert(harness.conversation().at(0).role() == Role::User);
    assert(harness.conversation().at(1).role() == Role::Assistant);

    // The sentinel is stored in the Conversation
    assert(harness.conversation().at(1).content() ==
           "This is the final response.<|end_conversation|>");

    // The sentinel should not be printed
    assert(output.output_.find("<|end_conversation|>") ==
           std::string::npos);
}





// Transcript Round-Trip
{
    ReplayModelClient replay("tests/p2/round_trip.txt");

    // Make sure the System message was loaded correctly
    assert(replay.system_message() == "You are a helpful assistant.");

    Conversation replay_conversation;
    replay_conversation.append(
        Message(Role::System, replay.system_message())
    );

    // First recorded turn
replay_conversation.append(
    Message(Role::User, "Hello")
);

Message reply1 = replay.generate(replay_conversation);

assert(reply1.role() == Role::Assistant);
assert(reply1.content() == "Hi there!");

replay_conversation.append(reply1);

// Second recorded turn
replay_conversation.append(
    Message(Role::User, "Goodbye")
);

Message reply2 = replay.generate(replay_conversation);

assert(reply2.role() == Role::Assistant);
assert(reply2.content() == "See you later!");

replay_conversation.append(reply2);

    // Verify the reconstructed conversation
    assert(replay_conversation.size() == 5);
    assert(replay_conversation.at(0).content() ==
           "You are a helpful assistant.");
    assert(replay_conversation.at(1).content() == "Hello");
    assert(replay_conversation.at(2).content() == "Hi there!");
    assert(replay_conversation.at(3).content() == "Goodbye");
    assert(replay_conversation.at(4).content() == "See you later!");
}

    return 0;
    

}
