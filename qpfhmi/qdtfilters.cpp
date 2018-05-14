/******************************************************************************
 * File:    qdtfilters.cpp
 *          Implementation of class Qdtfilters
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
 *   Implementation of structures used in QDT report filtering
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

#include "qdtfilters.h"

#define T(a,b) QString( b )
const QString ComparisonToken[] = { LIST_OF_COMPARISONS };
#undef T

const QString ProductSelToken[] = { "@AllProducts",
                                    "@SelectedProducts",
                                    "@ProductsOfType",
                                    "@ProductsInDateRange" };

QString QDTSingleFilter::evaluate()
{
    QString f;
    if (!section.isEmpty()) { f += QString("%1").arg(section); }
    if (!diag.isEmpty())    { f += QString(".%1").arg(diag); }
    if (!outGrp.isEmpty())  { f += QString(".%1").arg(outGrp); }
    if (!item.isEmpty())    { f += QString(".%1").arg(item); }

    QString flt("[" + f + "] ");
    switch (cmp) {
    case LT:
    case LE:
    case EQ:
    case NE:
    case GE:
    case GT:
        flt += ComparisonToken[(int)(cmp)] + QString(" %1").arg(cmpString);
        break;
    case CONTAINS:
        flt += "LIKE '%" + cmpString + "%'";
        break;
    case NOTCONTAINS:
        flt += "NOT LIKE '%" + cmpString + "%'";
        break;
    case BEGIN_WITH:
        flt += "LIKE '" + cmpString + "%'";
        break;
    case END_WITH:
        flt += "LIKE '%" + cmpString + "'";
        break;
    case MATCH:
        flt += "LIKE '" + cmpString + "'";
        break;
    default:
        break;
    }

    return flt;
}

QString QDTFilter::evaluate()
{
    static const QString FormatDateTime("yyyy-MM-dd hh:mm:ss");
    
    QString evalFilt("SELECT * FROM " + ProductSelToken[(int)(prodSelType)]);

    switch (prodSelType) {
    case ALL:
    case CURRENT:
        evalFilt += " ";
        break;
    case PROD_TYPE:
        evalFilt += QString("(%1) ").arg(prodType);
        break;
    case DATE_RANGE:
        evalFilt += QString("('%1'..'%2') ")
            .arg(dtFrom.toString(FormatDateTime))
            .arg(dtTo.toString(FormatDateTime));
        break;
    default:
        break;
    }

    evalFilt += "WHERE ";

    QString joint(criteria == ALL_OF_THEM ? "AND" : "OR");
    
    foreach (QDTSingleFilter flt, filts) {
        evalFilt += "{" + flt.evaluate() + "} " + joint + " ";                   
    }

    evalFilt.remove(evalFilt.length() - joint.length() - 2, 10);
    evalFilt += ";";

    return evalFilt;
}                          

