-- Keep a log of any SQL queries you execute as you solve the mystery.
-- I check the schema to see what columns are inside the tables
.schema
-- I query the crime_scene_reports table to check for the report of that day and on this street
SELECT description FROM crime_scene_reports
WHERE year = 2020 AND month = 7 AND day = 28 AND street LIKE '%Chamberlin%';
-- I query the interviews table to look for the three interviews mentioned in the report
SELECT name, transcript FROM interviews
WHERE year = 2020 AND month = 7 AND day = 28
AND transcript LIKE '%courthouse%';
-- Following first testimony we check the courthouse_security_logs and hope to find a license plate
SELECT license_plate FROM courthouse_security_logs
WHERE year = 2020 AND month = 7 AND day = 28 AND hour = 10 AND minute > 17 AND minute < 30;
-- Following the second interiew we look who might whitdraw money in front of the courthouse, will try to cros those dats with the license plates we already have
SELECT account_number FROM atm_transactions
WHERE year = 2020 AND month = 7 AND day = 28
AND transaction_type = 'withdraw' AND atm_location = 'Fifer Street';
-- We the decide to cross the two queries and hope to have an id that gets out
SELECT id FROM people WHERE license_plate IN
(
SELECT license_plate FROM courthouse_security_logs
WHERE year = 2020 AND month = 7 AND day = 28 AND hour = 10 AND minute > 17 AND minute < 30
)
INTERSECT
SELECT person_id FROM bank_accounts WHERE account_number IN
(
SELECT account_number FROM atm_transactions
WHERE year = 2020 AND month = 7 AND day = 28
AND transaction_type = 'withdraw' AND atm_location = 'Fifer Street'
);

-- As we get four ids from the querry bellow we need to check with the third interview.
-- However as the phone_calls table only state people's phone numbers we need to focus on this
SELECT caller FROM phone_calls WHERE caller IN
(
    SELECT phone_number FROM people WHERE license_plate IN
    (
        SELECT license_plate FROM courthouse_security_logs
        WHERE year = 2020 AND month = 7 AND day = 28 AND hour = 10 AND minute > 17 AND minute < 30
    )
    INTERSECT
    SELECT people.phone_number FROM bank_accounts
    JOIN people ON bank_accounts.person_id = people.id
    WHERE account_number IN
    (
        SELECT account_number FROM atm_transactions
        WHERE year = 2020 AND month = 7 AND day = 28
        AND transaction_type = 'withdraw' AND atm_location = 'Fifer Street'
    )
AND duration < 60 AND year = 2020 AND month = 7 AND day = 28
);
-- We now only have to suspects but one one too many then one result. We will now look for the passport numbers of those two people and look if they took a flight the following day
SELECT passport_number FROM passengers WHERE passport_number IN
(
    SELECT passport_number FROM people
    WHERE phone_number = '(367) 555-5533' OR phone_number = '(770) 555-1861'
)
AND flight_id IN
(
    SELECT id FROM flights
    WHERE year = 2020 AND month = 7 AND day = 29
    AND origin_airport_id = (SELECT id FROM airports WHERE city = 'Fiftyville')
);

-- As we still have two suspects we take look at the people they've called and try to figure out if this person could've bought a flight ticket the same day
-- We first get the ids of the callers:
SELECT name FROM people WHERE phone_number IN
(
    SELECT receiver FROM phone_calls WHERE caller IN
    (
        SELECT phone_number FROM people WHERE license_plate IN
        (
            SELECT license_plate FROM courthouse_security_logs
            WHERE year = 2020 AND month = 7 AND day = 28 AND hour = 10 AND minute > 17 AND minute < 30
        )
        INTERSECT
        SELECT people.phone_number FROM bank_accounts
        JOIN people ON bank_accounts.person_id = people.id
        WHERE account_number IN
        (
            SELECT account_number FROM atm_transactions
            WHERE year = 2020 AND month = 7 AND day = 28
            AND transaction_type = 'withdraw' AND atm_location = 'Fifer Street'
        )
    AND duration < 60 AND year = 2020 AND month = 7 AND day = 28
    )
);
-- Then we look a the transaction after the call
SELECT person_id, atm_transactions.* FROM atm_transactions
JOIN bank_accounts ON atm_transactions.account_number  = bank_accounts.account_number
WHERE bank_accounts.person_id = 847116 OR bank_accounts.person_id = 864400
AND atm_transactions.year = 2020 AND atm_transactions.month = 7 AND atm_transactions.day = 28
ORDER BY person_id;


-- We notice that only one withdraw money on the 29th, we also notice several operations around Fifer Street
-- Here are our sugestions:
-- FIETH :
SELECT name FROM people WHERE phone_number IN
(
    SELECT caller FROM phone_calls WHERE caller IN
    (
    SELECT phone_number FROM people WHERE license_plate IN
    (
        SELECT license_plate FROM courthouse_security_logs
        WHERE year = 2020 AND month = 7 AND day = 28 AND hour = 10 AND minute > 17 AND minute < 30
    )
    INTERSECT
    SELECT people.phone_number FROM bank_accounts
    JOIN people ON bank_accounts.person_id = people.id
    WHERE account_number IN
    (
        SELECT account_number FROM atm_transactions
        WHERE year = 2020 AND month = 7 AND day = 28
        AND transaction_type = 'withdraw' AND atm_location = 'Fifer Street'
    )
    AND duration < 60 AND year = 2020 AND month = 7 AND day = 28
    )
    AND receiver = (SELECT phone_number FROM people WHERE id = 847116)
);
-- Russell

-- ACCOMPLICE :
SELECT name FROM people WHERE id = 847116;
-- Philip

-- ESCAPTED TO :

SELECT city FROM airports WHERE id IN
(
    SELECT destination_airport_id FROM flights
    WHERE id IN
    (
        SELECT flight_id FROM passengers
        WHERE passport_number = (SELECT passport_number FROM people WHERE id = 514354)
    )
    AND year = 2020 AND month = 7 AND day = 29
    AND origin_airport_id = (SELECT id FROM airports WHERE city = 'Fiftyville')
);

-- As the check says i am wrong i guess the fieth is the second suspect. Still, we have nothing to prove it.
