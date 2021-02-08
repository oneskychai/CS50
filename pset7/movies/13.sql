SELECT name FROM people
WHERE id IN
    (SELECT people.id FROM movies JOIN
     stars ON movies.id = stars.movie_id JOIN
     people ON stars.person_id = people.id
     WHERE movies.id IN
        (SELECT movies.id FROM movies JOIN
         stars ON movies.id = stars.movie_id JOIN
         people ON stars.person_id = people.id
         WHERE name = "Kevin Bacon"
         and birth = 1958))
and NOT name = "Kevin Bacon";