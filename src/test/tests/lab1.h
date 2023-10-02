#ifndef __LAB1_H__
#define __LAB1_H__

#define API_VER 2
#include "framework.h"
#include <random>
#include <iomanip>

uint8_t verify_sum(lc3::sim &sim, uint16_t a, uint16_t b)
{
    uint8_t ret = 0b00;

    uint16_t unsigned_sum = (a & 0x00FF) + (b & 0x00FF), student_unsigned_sum = sim.readMem(0x3502);
    if (unsigned_sum & 0x0100)
        unsigned_sum = 0xFFFF;
    ret |= 0b01 * (unsigned_sum == student_unsigned_sum);

    uint16_t signed_sum = ((a >> 8) + (b >> 8)) << 8, student_signed_sum = sim.readMem(0x3503);
    if ((a & 0x8000) == (b & 0x8000) && (a & 0x8000) != (signed_sum & 0x8000))
        signed_sum = 0xFFFF;
    ret |= 0b10 * (signed_sum == student_signed_sum);

    return ret;
}

void ZeroTest(lc3::sim &sim, Tester &tester, double total_points)
{
    sim.writeMem(0x3500, 0x6FA2);
    sim.writeMem(0x3501, 0xB0B7);

    sim.run();

    tester.verify("ZeroTest", verify_sum(sim, 0x6FA2, 0xB0B7) & 0b11, total_points);
}

void Test(uint16_t a, uint16_t b, double frac, lc3::sim &sim, Tester &tester, double total_points)
{
    sim.writeMem(0x3500, a);
    sim.writeMem(0x3501, b);

    sim.run();

    uint8_t mask = verify_sum(sim, a, b);
    tester.verify("Testx3502", mask & 0b01, total_points * frac);
    tester.verify("Testx3503", mask & 0b10, total_points * (1 - frac));
}

void testBringup(lc3::sim &sim)
{
    sim.writePC(0x3000);
    sim.setRunInstLimit(1000);
}

void testTeardown(lc3::sim &sim) {}

void shutdown(void) {}

void lab1_setup(uint16_t num_tests, uint16_t seed, Tester &tester)
{
    std::mt19937 mt(seed);
    tester.registerTest("ZeroTest", ZeroTest, 1, true);

    for (uint16_t num_test = 0; num_test < num_tests; num_test++)
    {
        uint16_t A = mt() % 0x10000, B = mt() % 0x10000;
        std::stringstream stream;
        stream << std::hex << std::uppercase;

        auto test = [&stream, &tester](std::string test_name, uint16_t a, uint16_t b)
        {
            stream.str("");
            stream << "0x" << std::setfill('0') << std::setw(sizeof(uint16_t) * 2) << a;
            stream << "_";
            stream << "0x" << std::setfill('0') << std::setw(sizeof(uint16_t) * 2) << b;

            auto testUnsigned = [a, b](lc3::sim &sim, Tester &tester, double total_points)
            { Test(a, b, 0.8, sim, tester, total_points); };
            tester.registerTest(test_name + stream.str(), testUnsigned, 1, true);
        };

        test("TestUnsigned", A & 0x00FF, B & 0x00FF);
        test("TestSigned", A & 0xFF00, B & 0xFF00);
        test("TestBoth", A, B);
    }
}

#endif