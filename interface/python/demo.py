"""
    A tiny script to test the LapisParser Python interface
    llylly
"""
from LapisParser import *
import os
import pickle

secret_key = 'BA8FKxajrR9w47fmzala0ifgUcao7G'
pickle_path = 'sample.pkl'


def deletePkl():
    if os.path.exists(pickle_path):
        os.unlink(pickle_path)


def map_sample():
    if not os.path.exists(pickle_path):
        ret1 = addDocFromFile('../../map_server.yaml')
        assert ret1 is True

        ret2 = parseAPI()
        assert ret2 is True

        ret3 = getAPINames()
        # print(ret3)

        ret4 = parseScenario()
        print(getErrors())
        assert ret4 is True

        ret5 = getScenarioNames()
        # print(ret5)

        ret6 = parseConfig()
        assert ret6 is True

        test_report = runScenario(True)
        test_report = dict(test_report)

        with open(pickle_path, 'w') as f:
            pickle.dump(test_report, f)
    else:
        with open(pickle_path) as f:
            test_report = pickle.load(f)

    totTestCase = test_report['totTestCase']
    for i in range(totTestCase):
        now_case = test_report['testCases'][i]
        print('Case #{}'.format(i))
        terminate_reason = now_case['terminate']
        print('Terminate reason: {}'.format(terminate_reason))
        steps = now_case['steps']
        for step_no in range(len(steps)):
            print('Step {}'.format(step_no))
            step_entity = steps[step_no]
            if True:
                print('   module name: {}'.format(step_entity['moduleName']))
                print('   state name: {}'.format(step_entity['stateName']))
                if 'response' in step_entity:
                    print('   response: {}'.format(step_entity['response']))
                print('   sets:')
                sets = step_entity['sets']
                for set_name in sets:
                    print('      {}: {}'.format(set_name, sets[set_name]))

def post_sample():
    if not os.path.exists(pickle_path):
        ret1 = addDocFromFile('../../post_server.yaml')
        assert ret1 is True
        ret2 = parseAPI()
        assert ret2 is True
        ret3 = getAPINames()
        print(ret3)
        # baseline
        # for api in ret3:
        #     ret4 = runSingleAPI(api['name'], api['method'])
        #     print('name: {} method: {}\n    {}'.format(api['name'], api['method'], ret4))
        ret4 = parseScenario()
        print(getErrors())
        assert ret4 is True
        ret5 = getScenarioNames()
        print(ret5)
        ret6 = parseConfig()
        assert ret6 is True
        test_report = runScenario(1)
        test_report = dict(test_report)
        with open(pickle_path, 'w') as f:
            pickle.dump(test_report, f)
    else:
        with open(pickle_path) as f:
            test_report = pickle.load(f)

    totTestCase = test_report['totTestCase']
    totDetected = 0
    totLength = 0
    for i in range(totTestCase):
        now_case = test_report['testCases'][i]
        terminate_reason = now_case['terminate']['msg']
        totLength += len(now_case['steps'])
        if terminate_reason != 'Reach the end module "T".':
            totDetected += 1
    print('{} / {}'.format(totTestCase - totDetected, totTestCase))
    print('avg Length: {}'.format(float(totLength) / float(totTestCase)))

    return totTestCase - totDetected, float(totLength) / float(totTestCase)

    # totTestCase = test_report['totTestCase']
    # for i in range(totTestCase):
    #     now_case = test_report['testCases'][i]
    #     print('Case #{}'.format(i))
    #     terminate_reason = now_case['terminate']
    #     print('Terminate reason: {}'.format(terminate_reason))
    #     steps = now_case['steps']
    #     print('# of steps: {}'.format(len(steps)))
    #     for step_no in range(len(steps)):
    #         print('Step {}'.format(step_no))
    #         step_entity = steps[step_no]
    #         if True:
    #             print('   module name: {}'.format(step_entity['moduleName']))
    #             print('   state name: {}'.format(step_entity['stateName']))
    #             if 'response' in step_entity:
    #                 print('   response: {}'.format(step_entity['response']))
    #             print('   sets:')
    #             sets = step_entity['sets']
    #             for set_name in sets:
    #                 print('      {}: {}'.format(set_name, sets[set_name]))
    # print('Log:')
    # for log in getRuntimeLogs():
    #     print(log)


def ali_sample():
    ret1 = addDocFromFile('../../ali_sample.yaml')
    assert ret1 is True

    ret2 = parseAPI()
    assert ret2 is True

    ret3 = getAPINames()
    print(ret3)

    ret4 = parseScenario()
    assert ret4 is True

    ret5 = parseConfig()
    assert ret5 is True

    print(getScenarioNames())
    print(getConfig())

    if not os.path.exists(pickle_path):
        test_report = runScenario(2)
        test_report = dict(test_report)
        with open(pickle_path, 'w') as f:
            pickle.dump(test_report, f)
    else:
        with open(pickle_path) as f:
            test_report = pickle.load(f)


if __name__ == '__main__':
    deletePkl()
    # map_sample()
    post_sample()
    # ali_sample()
