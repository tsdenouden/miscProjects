-- Keep a log of any SQL queries you execute as you solve the mystery.

SELECT description FROM crime_scene_reports WHERE day = 28 AND month = 7 AND year = 2021 AND street = "Humphrey Street";
--Important details found:
--time of crime was 10:15 am
--crime took place at Humphrey Street Bakery
--Interviews were conducted with three witnesses
--Each witness mentioned the bakery
--Littering took place at 16:36

SELECT name,transcript FROM interviews WHERE day = 28 AND month = 7 AND year = 2021;

--Important details found:
--the thief drove away within ten minutes of the theft
--thief withdrew some money on Leggett Street
--thief called someone for less than a minute when leaving the baker
--person on the other end of the phone purchased a flight ticket for the earliest flight day after

-- Actions taken by thief:
--the thief -> drove away, withdrew money & called someone, then flew on the earliest flight


--People who drove away from the bakery on that day from 10:15 am to 10:25 am
SELECT name
FROM people
WHERE license_plate in
    (SELECT license_plate
    FROM bakery_security_logs
    WHERE hour = 10
    AND (minute >= 15 AND minute <= 25)
    AND activity = "exit"
    AND day = 28
    AND month = 7
    AND year = 2021);
--+---------+
--|  name   |
--+---------+
--| Vanessa |
--| Barry   |
--| Iman    |
--| Sofia   |
--| Luca    |
--| Diana   |
--| Kelsey  |
--| Bruce   |
--+---------+


--Person who withdrew some money on Leggett Street
SELECT name
FROM people
WHERE id in
    (SELECT person_id
    FROM bank_accounts
    WHERE account_number in
        (SELECT account_number
        FROM atm_transactions
        WHERE atm_location LIKE "Leggett Street"
        AND transaction_type = "withdraw"
        AND day = 28
        AND month = 7
        AND year = 2021));
--+---------+
--|  name   |
--+---------+
--| Kenny   |
--| Iman    |
--| Benista |
--| Taylor  |
--| Brooke  |
--| Luca    |
--| Diana   |
--| Bruce   |
--+---------+

--People who called for less than a minute
SELECT name
FROM people
WHERE phone_number IN
    (SELECT caller
    FROM phone_calls
    WHERE duration < 60 AND day = 28 AND month = 7 AND year = 2021);
--+---------+
--|  name   |
--+---------+
--| Kenny   |
--| Sofia   |
--| Benista |
--| Taylor  |
--| Diana   |
--| Kelsey  |
--| Bruce   |
--| Carina  |
--+---------+

-- POSSIBLE THIEF
--People who drove away from the bakery within ten minutes, withdrew money on Leggett Street & called someone for <min
-- Diana
-- Bruce


--People who received calls lasting less than a minute
SELECT name
FROM people
WHERE phone_number IN
    (SELECT receiver
    FROM phone_calls
    WHERE duration < 60 AND day = 28 AND month = 7 AND year = 2021);

--+------------+
--|    name    |
--+------------+
--| James      |
--| Larry      |
--| Anna       |
--| Jack       |
--| Melissa    |
--| Jacqueline |
--| Philip     |
--| Robin      |
--| Doris      |
--+------------+

--Possible thiefs: Diana & Bruce
--Their phone numbers
SELECT phone_number FROM people WHERE name = "Diana";
--Diana's phone number: (770) 555-1861
SELECT phone_number FROM people WHERE name = "Bruce";
--Bruce's phone number: (367) 555-5533

--ALL the callers & receivers on that day at that location for calls <min
SELECT caller,receiver FROM phone_calls WHERE duration < 60 AND day = 28 AND month = 7 AND year = 2021;
/*
+----------------+----------------+
|     caller     |    receiver    |
+----------------+----------------+
| (130) 555-0289 | (996) 555-8899 |
| (499) 555-9472 | (892) 555-8872 |
| (367) 555-5533 | (375) 555-8161 |
| (499) 555-9472 | (717) 555-1342 |
| (286) 555-6063 | (676) 555-6554 |
| (770) 555-1861 | (725) 555-3243 |
| (031) 555-6622 | (910) 555-3251 |
| (826) 555-1652 | (066) 555-9701 |
| (338) 555-6650 | (704) 555-2131 |
+----------------+----------------+*/

--Diana calls someone under the (725) 555-3243 phone number
SELECT name FROM people WHERE phone_number = "(725) 555-3243";
--Diana calls: Philip

--Bruce calls someone under the (375) 555-8161 phone number
SELECT name FROM people WHERE phone_number = "(375) 555-8161";
--Bruce calls: Robin

--Find the earliest flight's id
SELECT id,hour,minute FROM flights WHERE day = 29 AND month = 7 AND year = 2021 ORDER BY hour,minute;

--Name of everyone taking the earliest flight the day after theft
SELECT name
FROM people
WHERE passport_number in
    (SELECT passport_number
    FROM passengers
    WHERE flight_id = 36);
/*
+--------+
|  name  |
+--------+
| Kenny  |
| Sofia  |
| Taylor |
| Luca   |
| Kelsey |
| Edward |
| Bruce  |
| Doris  |
+--------+ */

--Diana is not taking this flight but Bruce is
--THIEF = BRUCE
--ACCOMPLICE = ROBIN

--Select the city of the destination airport of flight 36 (the earliest flight on 29th July 2021)
SELECT city FROM airports WHERE id in (SELECT destination_airport_id FROM flights WHERE id = 36);

--CITY = NEW YORK CITY