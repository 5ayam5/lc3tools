import unittest
from gradescope_utils.autograder_utils.decorators import partial_credit, visibility


class TestLab1(unittest.TestCase):
    def _get_score(self, file):
        print(file)
        try:
            with open(file, 'r') as f:
                lines = f.read()
                print(lines)
                if lines.splitlines()[-2].startswith('Total points earned:'):
                    return float(lines[-2].split()[-1][1:-2])
                else:
                    return 0.0
        except:
            return 0.0

    @partial_credit(30)
    @visibility('visible')
    def public_test_case(self, set_score=None):
        """Public test case"""
        set_score(self._get_score(
            '/autograder/submission/public.out') / 100 * 30)

    @partial_credit(50)
    @visibility('after_published')
    def private_test_case(self, set_score=None):
        """Private test case"""
        set_score(self._get_score(
            '/autograder/submission/private.out') / 100 * 80)
