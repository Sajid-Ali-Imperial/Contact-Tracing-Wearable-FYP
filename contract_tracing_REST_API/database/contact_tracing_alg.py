"""
Define algorithm constants. These are subject to change given more information relating to the spread of the virus.
"""
from datetime import datetime, timedelta


INCUBATION_DAYS = 3
TRANSMISSION_ORDER = 3
ISOLATION_DAYS = 10
DISTANCE_THRESHOLD = 2.0


"""
Devine algorithms to determine which users need to socially distance based on their exposure to individuals who have 
potentially contracted the virus. Start by identifying all the individuals that have tested positive for the virus.
Given an incubation time, find the individuals that have come into contact with a carrier after the incubation period.

"""
def updateCovidStatus(covid_tests, contacts, isolating_users):
    isolation_threshold = datetime.now() - timedelta(days=ISOLATION_DAYS)
    carriers = [] 
    # start with those who have tested positive in the last isolation days
    for uid in covid_tests.keys():
        latest_test = covid_tests[uid][-1]    # get latest test result
        latest_test_date = latest_test.keys()[0]
        latest_test_rslt = latest_test[latest_test_date]
        latest_test_date = datetime.strptime(latest_test_date, '%d-%m-%Y')
        if latest_test_date > isolation_threshold and latest_test_rslt == True:
            carriers.append(uid)
            # find all those that came into contact with infected individuals
            for date in get_dates_greater_than(contacts[uid].keys(), datetime.now()-timedelta(days=INCUBATION_DAYS)): 
                for contact_rslt in contacts[uid][date]:
                    users, dist = get_closest_distances(contact_rslt)


def get_dates_greater_than(dates, min_date):
    rslt = []
    for date in dates:
        dt_date = datetime.strptime(date, '%d-%m-%Y')
        if dt_date >= min_date:
            rslt.append()
    return rslt


"""Convert dictionary results to two list, one of the UIDs of the Users and the other of the closest distance"""
def get_closest_distances(contacts):
    users = contacts.keys().tolist()
    dist = []
    for user in users:
        dist.append(contacts[user])
    return users, dist
    






