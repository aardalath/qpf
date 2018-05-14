/******************************************************************************
 * File:    qdtfilters.h
 *          Declaration of class Qdtfilters
 *
 * Domain:  QPF.qpfgui.qdtfilters
 *
 * Version:  2.0
 *
 * Date:    2016-11-03
 *
 * Author:   J C Gonzalez
 *
 * Copyright (C) 2015-2018 Euclid SOC Team @ ESAC
 *_____________________________________________________________________________
 *
 * Topic: General Information
 *
 * Purpose:
 *   Declaration of structures used in QDT report filtering
 *
 * Created by:
 *   J C Gonzalez
 *
 * Status:
 *   Prototype
 *
 * Dependencies:
 *   none
 *
 * Files read / modified:
 *   none
 *
 * History:
 *   See <ChangeLog>
 *
 * About: License Conditions
 *   See <License>
 *
 ******************************************************************************/

#ifndef QDTFILTERS_H
#define QDTFILTERS_H

#include <QMap>
#include <QString>
#include <QDateTime>

class FrmDiagFlt;

enum ProductSelType { ALL, CURRENT, PROD_TYPE, DATE_RANGE };
enum MatchCriteria { ALL_OF_THEM, ANY_OF_THEM };

#undef T
#define LIST_OF_COMPARISONS                                     \
    T(LT, "<"),                                                 \
        T(LE, "<="),                                            \
        T(EQ, "=="),                                            \
        T(NE, "!="),                                            \
        T(GE, ">="),                                            \
        T(GT, ">"),                                             \
        T(CONTAINS, "Contains"),                                \
        T(NOTCONTAINS, "NotContains"),                          \
        T(BEGIN_WITH, "BeginsWith"),                            \
        T(END_WITH, "EndsWith"),                                \
        T(MATCH, "LIKE")

        /* T(TOP10, "InTop10"),                                    \ */
        /* T(BOTTOM10, "InBottom10"),                              \ */
        /* T(ABOVE_AVG, "IsAboveAvg"),                             \ */
        /* T(BELOW_AVG, "IsBelowAvg"),                             \ */

#define T(a,b) a
enum Comparison { LIST_OF_COMPARISONS };
#undef T

extern const QString ComparisonToken[];
extern const QString ProductSelToken[];

struct QDTSingleFilter {
    QDTSingleFilter() {
        section = "";
        diag = "";
        outGrp = "";
        item = "";
        cmp = LT;
        cmpString = "";
    }    
    QString section;
    QString diag;
    QString outGrp;
    QString item;
    Comparison cmp;
    QString cmpString;
    QString evaluate();
};
 
struct QDTFilter {
    QDTFilter() {
        prodSelType = ALL;
        prodType = "";
        criteria = ALL_OF_THEM;
        sqlQry = "";
    }
    ProductSelType prodSelType;
    QString prodType;
    QDateTime dtFrom, dtTo;
    MatchCriteria criteria;
    QMap<FrmDiagFlt*,QDTSingleFilter> filts;
    QString sqlQry;
    QString evaluate();
};                          
                                               
#endif // QDTFILTERS_H
