WITH
    sel AS ( SELECT val.description, val.type, f.id AS foo_id
             FROM ( VALUES
                    ('testing',     'blue'  ),
                    ('another row', 'red'   ),
                    ('new row1',    'purple'),
                    ('new row2',    'purple')
             ) val (description, type)
             LEFT JOIN foo f USING (type) ),
    ins AS ( INSERT INTO foo (type)
             SELECT DISTINCT type FROM sel WHERE foo_id IS NULL
             RETURNING id AS foo_id, type )
INSERT INTO bar (description, foo_id)
    SELECT sel.description, COALESCE(sel.foo_id, ins.foo_id)
    FROM   sel
    LEFT   JOIN ins USING (type);



