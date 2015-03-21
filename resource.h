/**
 * package: nodamysql
 * sub-package: resources
 * author:  Richard B. Winters <a href='mailto:rik@mmogp.com'>Rik At MMOGP</a>
 * copyright: 2011-2015 Massively Modified, Inc.
 * license: Apache, Version 2.0 <http://www.apache.org/licenses/LICENSE-2.0>
 */


// DEFINES
#define KWAERI_EMPTY            0
#define KWAERI_SELECT           1
#define KWAERI_INSERT           2
#define KWAERI_UPDATE           3
#define KWAERI_DELETE           4
#define KWAERI_NOPREP           5

#define KWAERI_NOT_PREP         0
#define KWAERI_CLAUSE_PREP      1
#define KWAERI_VALS_PREP_START  2
#define KWAERI_VALS_PREP_END    3
#define KWAERI_JOIN_PREP        4
#define KWAERI_WHERE_PREP_START 5
#define KWAERI_WHERE_PREP_END   6
#define KWAERI_LIMIT_PREP       7

#define KWAERI_TABLE_KEY        0
#define KWAERI_COLUMN_TYPE      1
#define KWAERI_COLUMN_TITLE     2

#define KWAERI_OPERATOR         0
#define KWAERI_WHERE_VALUE      1
#define KWAERI_WHERE_ADD        2
#define KWAERI_COLUMN_VALUE     2

#define KWAERI_PH_KEY           0
#define KWAERI_PH_VALUE         1
#define KWAERI_PH_TYPE          2
